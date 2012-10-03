#include "../../../Standard.h"
#include "../../../Boosters/Boosters.h"
#include "../../../Variables/IVariable.h"
#include "../../CovarianceModelUtilities.h"
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
				inline static int Oti(int i, int j)
				{
					_ASSERT(j <= i);
					return ((i * (i + 1)) >> 1) + j;
				}

				inline static int Ti(int i, int j)
				{
					return j <= i ? Oti(i, j) : Oti(j, i);
				}

				// Construction
				MultivariateIdentityModel::MultivariateIdentityModel(int nvars, int nlevels, const ImmutableVector<double> & theta)
					: ICovarianceModel((nvars * (nvars + 1)) >> 1, theta), nvars(nvars), nlevels(nlevels)
				{
					if (this->constant)
					{
						ValidateTheta(this->theta, theta);
						Copy(this->theta, theta);
					}
					else
					{
						for (int i = 0; i < this->nvars; ++i)
						{
							for (int j = 0; j < i; ++j)
								this->theta(Oti(i, j)) = 0.0;
							this->theta(Oti(i, i)) = 1.0;
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
								prec(i, j)(k) += this->theta(Oti(i, j));

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
								jac(i)(k) -= this->theta(Oti(i, j)) * beta(j)(k);

						// Strictly upper triangular part
						for (int j = i + 1; j < this->nvars; ++j)
							for (int k = 0; k < this->nlevels; ++k)
								jac(i)(k) -= this->theta(Oti(j, i)) * beta(j)(k);
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

				void MultivariateIdentityModel::ReparameterizeCoefficients(Vector<Vector<double> > & beta,
						const ImmutableVector<Pointer<Variables::IVariable> > & variables) const
				{
					for (int i = 0; i < this->nvars; ++i)
					{
						if (variables(i)->duplicate)
						{
							double num = 0.0;
							double den = 0.0;
							Vector<double> weights(this->nvars);
							for (int j = 0; j < this->nvars; ++j)
							{
								weights(j) = this->theta(Ti(i, j));
								den += Math::Square(weights(j));
								num += weights(j) * Mean(beta(j));
							}
							const double alpha = num / den;
							for (int j = 0; j < this->nvars; ++j)
								beta(j) -= alpha * weights(j);
						}
					}
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
