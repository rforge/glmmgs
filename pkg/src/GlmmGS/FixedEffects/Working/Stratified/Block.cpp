#include "../../../Standard.h"
#include "../../../Variables/IVariable.h"
#include "../../../Estimate.h"
#include "../../../Controls.h"
#include "Block.h"

namespace GlmmGS
{
	namespace FixedEffects
	{
		namespace Working
		{
			namespace Stratified
			{
				// Block
				Block::Block(Vector<Pointer<Variables::IVariable> > variables, WeakFactor factor)
					: variables(variables), factor(factor), beta(variables.Size())
				{
					for (int i = 0; i < this->variables.Size(); ++i)
						this->beta(i) = Vector<double>(this->factor.NumberOfLevels());
				}

				Block::~Block()
				{
				}

				Vector<Estimate> Block::Coefficients() const
				{
					const int nvars = this->variables.Size();
					const int nlevels = this->factor.NumberOfLevels();
					Vector<Estimate> y(nvars * nlevels);
					TriangularMatrix<Vector<double> > covariance = this->chol.Inverse();
					for (int index = 0, i = 0; i < nvars; ++i)
					{
						const Vector<double> & betai = this->beta(i);
						const Vector<double> & vari = covariance(i, i);
						for (int j = 0; j < nlevels; ++j, ++index)
							y(index) = Estimate(betai(j), vari(j));
					}
					return y;
				}

				// Methods
				void Block::UpdatePredictor(Vector<double> & eta) const
				{
					for (int j = 0; j < this->variables.Size(); ++j)
						this->variables(j)->UpdatePredictor(eta, this->beta(j), this->factor);
				}

				int Block::UpdateCoefficients(const Vector<double> & weights, const Vector<double> & values, const Controls & controls)
				{
					const int nvars = this->variables.Size();
					Vector<Vector<double> > jacobian(nvars);
					TriangularMatrix<Vector<double> > precision(nvars);
					for (int j = 0; j < nvars; ++j)
					{
						// Evaluate jacobian (return value optimization)
						Vector<double> ret_jacobian = Variables::ScalarProduct(this->variables(j), values, this->factor);
						Move(jacobian(j), ret_jacobian);
						// Evaluate precision (return value optimization)
						for (int k = 0; k <= j; ++k)
						{
							Vector<double> ret_precision =  Variables::ScalarProduct(this->variables(j), weights, this->variables(k), this->factor);
							Move(precision(j, k), ret_precision);
						}
					}
					// Decompose precision
					this->chol.Decompose(precision);

					// Evaluate update
					Vector<Vector<double> > h = this->chol.Solve(jacobian);

					// Print
					if (controls.Verbose())
						Print("Max update fixed effects: %g\n", MaxAbs(h));

					// Check if update is significant
					int update = 1;
					if (controls.Comparer().IsZero(h, this->beta))
						update = 0;

					// Update
					for (int i = 0; i < h.Size(); ++i)
						this->beta(i) += h(i);

					return update;
				}
			}
		}
	}
}
