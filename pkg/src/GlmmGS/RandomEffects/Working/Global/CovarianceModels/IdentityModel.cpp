#include "../../../../Standard.h"
#include "../../../../Estimate.h"
#include "IdentityModel.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Working
		{
			namespace Global
			{
				namespace CovarianceModels
				{
					// Construction
					IdentityModel::IdentityModel(int nvars)
						: size(nvars), tau(1.0)
					{
					}

					IdentityModel::~IdentityModel()
					{
					}

					// Properties
					Vector<Estimate> IdentityModel::Estimates() const
					{
						Vector<Estimate> estimates(1);
						estimates(0) = Estimate(sqrt(1.0 / this->tau), -1); // TODO: calculate variance
						return estimates;
					}

					// Methods
					void IdentityModel::Decompose(const TriangularMatrix<double> & design_precision)
					{
						// Add diagonal to precision
						TriangularMatrix<double> prec = design_precision;
						for (int i = 0; i < this->size; ++i)
							prec(i, i) += this->tau;

						// Decompose
						this->chol.Decompose(prec);
					}

					int IdentityModel::Update(const Vector<double> & beta, Comparer comparer)
					{
						// Calculate variance
						const TriangularMatrix<double> variance = this->chol.Inverse();

						// Calculate jacobian and minus the hessian
						const double bsquare = LinearAlgebra::Square(beta);
						const double trace = LinearAlgebra::Trace(variance);
						const double jac = this->size / this->tau - bsquare - trace;
						const double minus_hessian = this->size / Math::Square(this->tau) - LinearAlgebra::SquareTrace(variance);

						// Calculate update
						double h = jac / minus_hessian;
						const int update = comparer.IsZero(h, this->tau) ? 0 : 1;

						// Update sigma_square
						this->tau += h;
						while (this->tau <= 0.0)
						{
							// Back-track
							h *= 0.5;
							this->tau -= h;
						}

						// Debug
						Print("Max update covariance components= %g\n", h);

						return update;
					}

					Vector<double> IdentityModel::CoefficientsUpdate(const Vector<double> & jacobian, const Vector<double> & beta) const
					{
						// Add diagonal terms
						Vector<double> jac = jacobian;
						for (int i = 0; i < this->size; ++i)
							jac(i) -= this->tau * beta(i);

						// Decomposes
						return this->chol.Solve(jac);
					}
				}
			}
		}
	}
}
