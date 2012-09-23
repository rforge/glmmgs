#include "../../Standard.h"
#include "../../Control.h"
#include "../../Variables/IVariable.h"
#include "CovarianceModels/ICovarianceModel.h"
#include "Block.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Global
		{
			// Block
			Block::Block(const Vector<Pointer<Variables::IVariable> > & variables,
					const Pointer<CovarianceModels::ICovarianceModel> & covariance_model)
				: variables(variables),
				  beta(variables.Size()),
				  covariance_model(covariance_model)
			{
			}

			// Properties
			const ImmutableVector<double> & Block::Coefficients() const
			{
				return this->beta;
			}

			Pointer<CovarianceModels::ICovarianceModel> Block::CovarianceModel() const
			{
				return this->covariance_model;
			}

			// Implementation
			void Block::UpdatePredictor(Vector<double> & eta) const
			{
				for (int j = 0; j < this->variables.Size(); ++j)
					this->variables(j)->UpdatePredictor(eta, this->beta(j));
			}

			int Block::UpdateCoefficients(const ImmutableVector<double> & weights, const ImmutableVector<double> & values, const Control & control)
			{
				// Evaluate design Jacobian and design precision
				const int nvars = this->variables.Size();
				Vector<double> design_jacobian(nvars);
				TriangularMatrix<double> design_precision(nvars);
				for (int j = 0; j < nvars; ++j)
				{
					design_jacobian(j) = Variables::ScalarProduct(this->variables(j), values);
					for (int k = 0; k <= j; ++k)
						design_precision(j, k) = Variables::ScalarProduct(this->variables(j), weights, this->variables(k));
				}

				// Decompose full precision
				this->covariance_model->Decompose(design_precision);

				// Evaluate coefficients update
				Vector<double> h = covariance_model->CoefficientsUpdate(design_jacobian, this->beta);

				// Check if update is significant
				if (control.comparer.IsZero(h, this->beta))
					return 0;

				// Scale update
				ScaleUpdate(h, control.max_updates.ranef);

				// Update
				this->beta += h;

				// Print
				if (control.verbose)
					Print("Max update random effects: %g\n", MaxAbs(h));

				return 1;
			}

			int Block::UpdateCovarianceComponents(const ImmutableVector<double> & weights, const ImmutableVector<double> & values, const Control & control)
			{
				// Evaluate design precision
				const int nvars = this->variables.Size();
				TriangularMatrix<double> design_precision(nvars);
				for (int j = 0; j < nvars; ++j)
					for (int k = 0; k <= j; ++k)
						design_precision(j, k) = Variables::ScalarProduct(this->variables(j), weights, this->variables(k));

				// Decompose full precision
				this->covariance_model->Decompose(design_precision);

				// Update covariance components
				return this->covariance_model->UpdateComponents(this->beta, control);
			}
		}
	}
}
