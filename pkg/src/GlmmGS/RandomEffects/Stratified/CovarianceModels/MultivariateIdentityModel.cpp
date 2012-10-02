#include "../../../Standard.h"
#include "../../../Boosters/Boosters.h"
#include "../../../Variables/IVariable.h"
#include "../../MultivariateCovarianceModelUtilities.h"
#include "MultivariateIdentityModel.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Stratified
		{
			namespace CovarianceModels
			{
				// Helper - TODO: Use Ti instead
				inline
				static int Index(int i, int j)
				{
					_ASSERT_ARGUMENT(j <= i);
					return ((i * (i + 1)) >> 1) + j;
				}

				// Construction
				MultivariateIdentityModel::MultivariateIdentityModel(int nvars, int nlevels, const ImmutableVector<double> & theta)
					: ICovarianceModel((nvars * (nvars + 1)) >> 1, theta), nvars(nvars), nlevels(nlevels)
				{
					if (this->constant)
					{
						Copy(this->theta, theta);
					}
					else
					{
						for (int i = 0; i < this->nvars; ++i)
						{
							for (int j = 0; j < i; ++j)
								this->theta(Index(i, j)) = 0.0;
							this->theta(Index(i, i)) = 1.0;
						}
					}
				}

				MultivariateIdentityModel::~MultivariateIdentityModel()
				{
				}

				// Coefficients
				void MultivariateIdentityModel::Decompose(const TriangularMatrix<Vector<double> > & design_precision)
				{
					// Update precision matrix
					TriangularMatrix<Vector<double> > prec = design_precision;
					for (int i = 0; i < this->nvars; ++i)
						for (int j = 0; j <= i; ++j)
							for (int k = 0; k < this->nlevels; ++k)
								prec(i, j)(k) += this->theta(Index(i, j));

					// Decompose
					this->beta_precision_chol.Decompose(prec);
				}

				Vector<Vector<double> > MultivariateIdentityModel::CoefficientsUpdate(const Vector<Vector<double> > & jacobian, const Vector<Vector<double> > & beta) const
				{
					// Add diagonal terms
					Vector<Vector<double> > jac = jacobian;
					for (int i = 0; i < this->nvars; ++i)
					{
						// Lower triangular part
						for (int j = 0; j <= i; ++j)
							for (int k = 0; k < this->nlevels; ++k)
								jac(i)(k) -= this->theta(Index(i, j)) * beta(j)(k);

						// Strictly upper triangular part
						for (int j = i + 1; j < this->nvars; ++j)
							for (int k = 0; k < this->nlevels; ++k)
								jac(i)(k) -= this->theta(Index(j, i)) * beta(j)(k);
					}

					// Decomposes
					return this->beta_precision_chol.Solve(jac);
				}

				// TODO: move
				TriangularMatrix<double> ThetaToPrecision(const ImmutableVector<double> & theta, int nvars)
				{
					TriangularMatrix<double> tau(nvars);
					for (int ij = 0, i = 0; i < nvars; ++i)
						for (int j = 0; j <= i; ++j, ++ij)
							tau(i, j) = theta(ij);
					return tau;
				}

				Vector<Vector<double> > operator * (const ImmutableTriangularMatrix<double> & tau, const Vector<Vector<double> > & u)
				{
					const int nvars = u.Size();
					const int nlevels = u(0).Size();
					Vector<Vector<double> > y = VectorVector<double>(nvars, nlevels);
					for (int i = 0; i < nvars; ++i)
					{
						// Lower
						for (int j = 0; j <= i; ++j)
							for (int k = 0; k < nlevels; ++k)
								y(i)(k) += tau(i, j) * u(j)(k);
						// Upper
						for (int j = i + 1; j < tau.NumberOfRows(); ++j)
							y(i) += tau(j, i) * LinearAlgebra::Sum(u(j));
					}
					return y;
				}

				inline
				void PlusEqual(double & x, double alpha, double v)
				{
					x += alpha * v;
				}

				template <class TYPE>
				void PlusEqual(Vector<TYPE> & x, double alpha, const Vector<TYPE> & v)
				{
					_ASSERT_ARGUMENT(x.Size() == v.Size());
					for (int i = 0; i < x.Size(); ++i)
						PlusEqual(x(i), alpha, v(i));
				}

				void OrthogonalProjection(Vector<Vector<double> > & x, const Vector<Vector<double> > & v)
				{
					const double alpha = -LinearAlgebra::ScalarProduct(x, v) / LinearAlgebra::ScalarProduct(v, v);
					PlusEqual(x, alpha, v);
				}

				void MultivariateIdentityModel::ReparameterizeCoefficients(Vector<Vector<double> > & beta,
						const ImmutableVector<Pointer<Variables::IVariable> > & variables) const
				{
					Boosters::Reparameterize(beta, variables, this->remove_mean);
					//Vector<Vector<double> > u = Utilities::VectorVector<double>(beta.Size(), beta(0).Size());
					//Utilities::Set<double>(u(0), 1.0);

					//const ImmutableTriangularMatrix<double> tau = ThetaToPrecision(this->theta, this->nvars);
					//const Vector<Vector<double> > v = tau * u;
					//OrthogonalProjection(beta, v);
				}

				// Components
				int MultivariateIdentityModel::UpdateComponentsImpl(const Vector<Vector<double> > & beta, const Control & control)
				{
					// Calculate variances
					const ImmutableTriangularMatrix<Vector<double> > variance = this->beta_precision_chol.Inverse();
					const ImmutableTriangularMatrix<double> sigma = ThetaToVariance(this->theta, this->nvars);

					// Calculate traces and scalar products
					FTrace<double> sigmaTrace(sigma);
					FTrace<Vector<double> > varianceTrace(variance);
					FSquareTrace<double> sigmaSquareTrace(sigma);
					FSquareTrace<Vector<double> > varianceSquareTrace(variance);
					FProduct betaProduct(beta);

					// Calculate jacobian and minus the hessian
					const int npars = this->theta.Size();
					Vector<double> jac(npars);
					TriangularMatrix<double> minus_hessian(npars);
					for (int i = 0; i < npars; ++i)
					{
						jac(i) = this->nlevels * sigmaTrace(i) - betaProduct(i) - varianceTrace(i);
						for (int j = 0; j <= i; ++j)
							minus_hessian(i, j) = this->nlevels * sigmaSquareTrace(i, j) - varianceSquareTrace(i, j);
					}

					// Update covariance components
					return ICovarianceModel::UpdateComponents(minus_hessian, jac, control);
				}
			}
		}
	}
}
