#include "ICovarianceModel.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Working
		{
			namespace Stratified
			{
				namespace CovarianceModels
				{
					// Construction
					ICovarianceModel::ICovarianceModel(int nvars)
						: nvars(nvars), theta(nvars)
					{
					}

					ICovarianceModel::~ICovarianceModel()
					{
					}

					// Properties
					Vector<Estimate> ICovarianceModel::Estimates() const
					{
						Vector<Estimate> estimates(nvars);
						TriangularMatrix<double> covariance = this->chol.Inverse();
						for (int j = 0; j < nvars; ++j)
							estimates(j) = Estimate(this->theta(j), covariance(j, j));
						return estimates;
					}

					int ICovarianceModel::Update(const TriangularMatrix<double> & minus_hessian,
							const Vector<double> & jacobian, Comparer comparer)
					{
						// Calculate update
						try
						{
							this->chol.Decompose(minus_hessian);
							Vector<double> h = chol.Solve(jacobian);
							const int update = comparer.IsZero(h, this->theta) ? 0 : 1;

							// Debug
							Print("MaxAbs covariance components: %g\n", MaxAbs(h));

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
							throw Exceptions::Exception("Failed to update covariance components");
							return 1;
						}
					}
				}
			}
		}
	}
}
