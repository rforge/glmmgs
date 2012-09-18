#include "../../Standard.h"
#include "../../Variables/IVariable.h"
#include "../../Controls.h"
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
			const Vector<Vector<double> > & Block::Coefficients() const
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
				const int nvars = this->variables.Size();
				for (int j = 0; j < nvars; ++j)
					this->variables(j)->UpdatePredictor(eta, this->beta(j), this->factor);
			}

			int Block::Update(const Vector<double> & weights, const Vector<double> & values, const Controls & controls)
			{
				const int nvars = this->variables.Size();
				TriangularMatrix<Vector<double> >precision(nvars);
				Vector<Vector<double> > jacobian(nvars);
				for (int j = 0; j < nvars; ++j)
				{
					// Evaluate jacobian
					jacobian(j) = Variables::ScalarProduct(this->variables(j), values, this->factor);
					for (int k = 0; k <= j; ++k)
					{
						// Evaluate precision
						precision(j, k) =  Variables::ScalarProduct(this->variables(j), weights, this->variables(k), this->factor);
					}
				}

				// Decompose precision
				this->covariance_model->Decompose(precision);

				// Re-parameterize coefficients
				this->covariance_model->ReparameterizeCoefficients(this->beta, this->variables);

				// Evaluate coefficients update
				Vector<Vector<double> > h = this->covariance_model->CoefficientsUpdate(jacobian, this->beta);

				// Re-parameterize updates
				this->covariance_model->ReparameterizeCoefficients(h, this->variables);

				// Check if update is significant
				int update = controls.Comparer().IsZero(h, this->beta) ? 0 : 1;

				// Print
				if (controls.Verbose())
					Print("Max update random effects: %g\n", MaxAbs(h));

				// Update
				this->beta += h;

				// Update covariance components
				update += this->covariance_model->Update(this->beta, controls);

				return update;
			}
		}
	}
}
