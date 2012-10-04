#include "../../Standard.h"
#include "../../Variables/IVariable.h"
#include "../../Control.h"
#include "../../FixedEffects/IBlock.h"
#include "Block.h"
#include "CovarianceModels/ICovarianceModel.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Stratified
		{
			// Block
			Block::Block(const Vector<Pointer<Variables::IVariable> > & variables,
					WeakFactor factor,
					const Pointer<CovarianceModels::ICovarianceModel> & covariance_model)
				: variables(variables),
				  factor(factor),
				  beta(VectorVector<double>(variables.Size(), this->factor.NumberOfLevels())),
				  covariance_model(covariance_model)
			{
			}

			// Properties
			const ImmutableVector<Vector<double> > & Block::Coefficients() const
			{
				return this->beta;
			}

			Pointer<CovarianceModels::ICovarianceModel> Block::CovarianceModel() const
			{
				return this->covariance_model;
			}

			// Implementation
			void Block::EvaluateLinearDependencies(const Pointer<FixedEffects::IBlock> & fixef)
			{
				// Simple linear 1 to 1 linear dependencies
				//for (int i = 0; i < this->variables.Size(); ++i)
				//{
				//}
			}

			void Block::UpdatePredictor(Vector<double> & eta) const
			{
				const int nvars = this->variables.Size();
				for (int j = 0; j < nvars; ++j)
					this->variables(j)->UpdatePredictor(eta, this->beta(j), this->factor);
			}

			int Block::UpdateCoefficients(const ImmutableVector<double> & weights, const ImmutableVector<double> & values, const Control & control)
			{
				// Evaludate design jacobian and design precision
				const int nvars = this->variables.Size();
				Vector<Vector<double> > design_jacobian(nvars);
				TriangularMatrix<Vector<double> >design_precision(nvars);
				for (int j = 0; j < nvars; ++j)
				{
					design_jacobian(j) = Variables::ScalarProduct(this->variables(j), values, this->factor);
					for (int k = 0; k <= j; ++k)
						design_precision(j, k) =  Variables::ScalarProduct(this->variables(j), weights, this->variables(k), this->factor);
				}

				// Decompose full precision
				this->covariance_model->Decompose(design_precision);

				// Evaluate coefficients update
				Vector<Vector<double> > h = this->covariance_model->CoefficientsUpdate(design_jacobian, this->beta);

				// Re-parameterize updates
				//this->covariance_model->ReparameterizeCoefficients(h, this->variables);

				// Check if update is significant
				const int update = control.comparer.IsZero(h, this->beta) ? 0 : 1;

				// Scale update
				ScaleUpdate(h, control.max_updates.ranef);

				// Update
				this->beta += h;

				// Re-parameterize coefficients
				this->covariance_model->ReparameterizeCoefficients(this->beta, this->variables);

				// Print
				if (control.verbose)
					Print("Max update random effects: %g\n", MaxAbs(h));

				return update;
			}

			int Block::UpdateCovarianceComponents(const ImmutableVector<double> & weights, const ImmutableVector<double> & values, const Control & control)
			{
				// Evaluate design precision
				const int nvars = this->variables.Size();
				TriangularMatrix<Vector<double> >design_precision(nvars);
				for (int j = 0; j < nvars; ++j)
					for (int k = 0; k <= j; ++k)
						design_precision(j, k) = Variables::ScalarProduct(this->variables(j), weights, this->variables(k), this->factor);

				// Decompose full precision
				this->covariance_model->Decompose(design_precision);

				// Update covariance components
				return this->covariance_model->UpdateComponents(this->beta, control);
			}
		}
	}
}
