#include "../../../../Standard.h"
#include "../../../../Estimate.h"
#include "IdentityModel.h"

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
					IdentityModel::IdentityModel(int nvars, int nlevels)
						: nvars(nvars), nlevels(nlevels), tau(nvars)
					{
						this->tau = 1.0;
					}

					IdentityModel::~IdentityModel()
					{
					}

					// Properties
					Vector<Estimate> IdentityModel::Estimates() const
					{
						Vector<Estimate> estimates(nvars);
						for (int j = 0; j < nvars; ++j)
							estimates(j) = Estimate(sqrt(1.0 / this->tau(j)), -1); // TODO: calculate variance
						return estimates;
					}

					// Methods
					void IdentityModel::Decompose(const TriangularMatrix<Vector<double> > & design_precision)
					{
						// Add diagonal to precision
						TriangularMatrix<Vector<double> > prec = design_precision;
						for (int i = 0; i < this->nvars; ++i)
						{
							Vector<double> & prec_i = prec(i, i);
							const double tau_i = this->tau(i);
							for (int k = 0; k < this->nlevels; ++k)
								prec_i(k) += tau_i;
						}

						// Decompose
						this->vchol.Decompose(prec);
					}

					int IdentityModel::Update(const Vector<Vector<double> > & beta, Comparer comparer)
					{
						// Calculate variance
						const TriangularMatrix<Vector<double> > variance = this->vchol.Inverse();

						// Calculate jacobian and minus the hessian
						Vector<double> jac(this->nvars);
						TriangularMatrix<double> minus_hessian(this->nvars);
						for (int i = 0; i < this->nvars; ++i)
						{
							const double bsquare = Square(beta(i));
							const double trace = Sum(variance(i, i));
							jac(i) = this->nlevels / this->tau(i) - bsquare - trace;
							minus_hessian(i, i) = this->nlevels / Square(this->tau(i)) - Square(variance(i, i));
							for (int j = 0; j < i; ++j)
								minus_hessian(i, j) = -Square(variance(i, j));
						}

						// Calculate update
						try
						{
							CholeskyDecomposition chol(minus_hessian);
							Vector<double> h = chol.Solve(jac);
							const int update = comparer.IsZero(h, this->tau) ? 0 : 1;

							// Debug
							Print("MaxAbs covariance components: %g\n", MaxAbs(h));

							// Update tau
							this->tau += h;

							// Check sign
							while (Min(this->tau) <= 0.0)
							{
								// Back-track
								h *= 0.5;
								this->tau -= h;
							}

							return update;
						}
						catch(Exceptions::Exception &)
						{
							return 1;
						}
					}

					Vector<Vector<double> > IdentityModel::CoefficientsUpdate(const Vector<Vector<double> > & jacobian, const Vector<Vector<double> > & beta) const
					{
						// Add diagonal terms
						Vector<Vector<double> > jac = jacobian;
						for (int i = 0; i < this->nvars; ++i)
						{
							Vector<double> & jaci = jac(i);
							const Vector<double> & betai = beta(i);
							const double taui = this->tau(i);
							for (int k = 0; k < this->nlevels; ++k)
								jaci(k) -= taui * betai(k);
						}

						// Decomposes
						return this->vchol.Solve(jac);
					}
				}
			}
		}
	}
}
