#include "../../Standard.h"
#include "../../Controls.h"
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

			int Block::Update(const ImmutableVector<double> & weights, const ImmutableVector<double> & values, const Controls & controls)
			{
				const int nvars = this->variables.Size();
				Vector<double> jacobian(nvars);
				TriangularMatrix<double> precision(nvars);
				for (int j = 0; j < nvars; ++j)
				{
					// Evaluate jacobian
					jacobian(j) = Variables::ScalarProduct(this->variables(j), values);
					// Evaluate precision
					for (int k = 0; k <= j; ++k)
						precision(j, k) = Variables::ScalarProduct(this->variables(j), weights, this->variables(k));
				}

				// Decompose precision
				this->covariance_model->Decompose(precision);

				// Evaluate coefficients update
				Vector<double> h = covariance_model->UpdateCoefficients(jacobian, this->beta);

				// Print
				if (controls.Verbose())
					Print("Max update random effects: %g\n", MaxAbs(h));

				// Check if update is significant
				int update = controls.Comparer().IsZero(h, this->beta) ? 0 : 1;

				// Update coefficients
				this->beta += h;

				// Update covariance model
				update += this->covariance_model->Update(this->beta, controls);

				return update;
			}
		}
	}
}
