#include "../../Standard.h"
#include "../../Variables/IVariable.h"
#include "../../Controls.h"
#include "Block.h"

namespace GlmmGS
{
	namespace FixedEffects
	{
		namespace Global
		{
			// Block
			Block::Block(Vector<Pointer<Variables::IVariable> > variables)
				: variables(variables), beta(variables.Size())
			{
			}

			Block::~Block()
			{
			}

			// Properties
			const Vector<double> & Block::Coefficients() const
			{
				return this->beta;
			}

			TriangularMatrix<double> Block::Covariance() const
			{
				return this->chol.Inverse();
			}

			// Methods
			void Block::UpdatePredictor(Vector<double> & eta) const
			{
				const int nvars = this->variables.Size();
				for (int j = 0; j < nvars; ++j)
					this->variables(j)->UpdatePredictor(eta, this->beta(j));
			}

			int Block::UpdateCoefficients(const Vector<double> & weights, const Vector<double> & values, const Controls & controls)
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
				this->chol.Decompose(precision);

				// Evaluate update
				Vector<double> h = this->chol.Solve(jacobian);

				// Print
				if (controls.Verbose())
					Print("Max update fixed effects: %g\n", MaxAbs(h));

				// Check if update is significant
				const int update = controls.Comparer().IsZero(h, this->beta) ? 0 : 1;

				// Update
				this->beta += h;

				return update;
			}
		}
	}
}
