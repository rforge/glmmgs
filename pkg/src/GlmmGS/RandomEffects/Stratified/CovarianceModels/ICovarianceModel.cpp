#include "ICovarianceModel.h"
#include "../../../Controls.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Stratified
		{
			namespace CovarianceModels
			{
				// Construction
				ICovarianceModel::ICovarianceModel(int npars)
					: theta(npars)
				{
				}

				ICovarianceModel::~ICovarianceModel()
				{
				}

				// Properties
				const Vector<double> & ICovarianceModel::Components() const
				{
					return this->theta;
				}

				TriangularMatrix<double> ICovarianceModel::Covariance() const
				{
					return this->chol.Inverse();
				}

				// Implementation
				int ICovarianceModel::Update(const TriangularMatrix<double> & minus_hessian,
						const Vector<double> & jacobian, const Controls & controls)
				{
					// Calculate update
					try
					{
						this->chol.Decompose(minus_hessian);
						Vector<double> h = chol.Solve(jacobian);
						const int update = controls.Comparer().IsZero(h, this->theta) ? 0 : 1;

						// Debug
						if (controls.Verbose())
							Print("Max update covariance components: %g\n", MaxAbs(h));

						// Update tau
						this->theta += h;

						// Check sign
						while (Min(this->theta) <= 0.0)
						{
							// Back-track
							h *= 0.5;
							this->theta -= h;
						}

						return update;
					}
					catch(Exceptions::Exception &)
					{
						//throw Exceptions::Exception("Failed to update covariance components");
						Print("Warning: Failed to update covariance components");
					}

					// Here only if update failed. Keep updating
					return 1;
				}
			}
		}
	}
}
