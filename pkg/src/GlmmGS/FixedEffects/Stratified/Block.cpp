#include "../../Standard.h"
#include "../../Variables/IVariable.h"
#include "../../Controls.h"
#include "../../Boosters/Boosters.h"
#include "Block.h"

namespace GlmmGS
{
	namespace FixedEffects
	{
		namespace Stratified
		{
			// Block
			Block::Block(Vector<Pointer<Variables::IVariable> > variables, WeakFactor factor)
				: variables(variables),
				  factor(factor),
				  beta(VectorVector<double>(this->variables.Size(), this->factor.NumberOfLevels()))
			{
			}

			Block::~Block()
			{
			}

			// Properties
			const Vector<Vector<double> > & Block::Coefficients() const
			{
				return this->beta;
			}

			TriangularMatrix<Vector<double> > Block::Covariance() const
			{
				return this->chol.Inverse();
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
					// Evaluate jacobian
					jacobian(j) = Variables::ScalarProduct(this->variables(j), values, this->factor);
					// Evaluate precision
					for (int k = 0; k <= j; ++k)
						precision(j, k) =  Variables::ScalarProduct(this->variables(j), weights, this->variables(k), this->factor);
				}
				// Decompose precision
				this->chol.Decompose(precision);

				// Evaluate update
				Vector<Vector<double> > h = this->chol.Solve(jacobian);

				// Re-parameterize updates
				//Boosters::Reparameterize(h, this->variables, Boosters::RemoveFirstComponent());

				// Print
				if (controls.Verbose())
					Print("Max update fixed effects: %g\n", MaxAbs(h));

				// Check if update is significant
				const int update = controls.Comparer().IsZero(h, this->beta) ? 0 : 1;

				// Update
				this->beta += h;

				// Re-parameterize coefficients
				//Boosters::Reparameterize(this->beta, this->variables, Boosters::RemoveFirstComponent());

				return update;
			}
		}
	}
}
