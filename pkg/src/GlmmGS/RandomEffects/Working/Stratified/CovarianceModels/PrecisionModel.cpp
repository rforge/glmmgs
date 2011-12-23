#include "../../../../Standard.h"
#include "../../../../Estimate.h"
#include "PrecisionModel.h"
#include "Functions.h"

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
					PrecisionModel::PrecisionModel(int nvars, WeakMatrix<const double> R)
						: nvars(nvars), R(R), tau(nvars)
					{
						this->tau = 1.0;
					}

					PrecisionModel::~PrecisionModel()
					{
					}

					// Properties
					Vector<Estimate> PrecisionModel::Estimates() const
					{
						Vector<Estimate> estimates(nvars);
						for (int j = 0; j < nvars; ++j)
							estimates(j) = Estimate(this->tau(j), 0.0); // TODO: calculate variance
						return estimates;
					}

					// Methods
					void PrecisionModel::Decompose(const TriangularMatrix<Vector<double> > & design_precision)
					{
						// Add diagonal to precision
						const int nlevels = this->R.NumberOfRows();
						TriangularMatrix<double> prec(this->nvars * nlevels);
						for (int i = 0; i < this->nvars; ++i)
						{
							const double tau_i = this->tau(i);
							const Vector<double> & design_precision_i = design_precision(i, i);
							const int offset_i = i * nlevels;

							// Diagonal block
							for (int k = 0; k < nlevels; k++)
							{
								prec(offset_i + k, offset_i + k) = design_precision_i(k) + tau_i * this->R(k, k);
								for (int l = 0; l < k; ++l)
									prec(offset_i + k, offset_i + l) = tau_i * this->R(k, l);
							}

							// Off-diagonal blocks
							for (int j = 0; j < i; ++j)
							{
								const int offset_j = j * nlevels;
								const Vector<double> & design_precision_ij = design_precision(i, j);
								for (int k = 0; k < nlevels; k++)
									prec(offset_i + k, offset_j + k) += design_precision_ij(k);
							}
						}

						// Decompose
						this->chol.Decompose(prec);
					}

					int PrecisionModel::Update(const Vector<Vector<double> > & beta, Comparer comparer)
					{
						// Calulate T^{-1} R
						const int nlevels = this->R.NumberOfColumns();
						const int size = this->nvars * nlevels;
						Matrix<double> a(size, size);
						Vector<double> b(size);
						for (int i = 0, ik = 0; i < this->nvars; ++i)
						{
							for (int k = 0; k < nlevels; ++k, ++ik)
							{
								// Prepare b
								b = 0.0;
								const int offset = i * nlevels;
								for (int l = 0; l < nlevels; ++l)
									b(offset + l) = this->R(l, k);

								// Solve T_j x = b
								Vector<double> x = this->chol.Solve(b);

								// Store x
								for (int j = 0; j < size; ++j)
									a(j, ik) = x(j);
							}
						}

						// Calculate jacobian and minus the hessian
						Vector<double> jac(this->nvars);
						TriangularMatrix<double> minus_hessian(this->nvars);
						for (int i = 0; i < this->nvars; ++i)
						{
							const double bsquare = Square(this->R, beta(i));
							jac(i) = nlevels / this->tau(i) - bsquare - BlockTrace(i, nlevels, a);
							minus_hessian(i, i) = nlevels / Math::Square(this->tau(i)) - BlockSquareTrace(i, i, nlevels, a);
							for (int j = 0; j < i; ++j)
								minus_hessian(i, j) = -BlockSquareTrace(i, j, nlevels, a);
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
							throw Exceptions::Exception("Failed to update covariance components");
							return 1;
						}
					}

					Vector<Vector<double> > PrecisionModel::CoefficientsUpdate(const Vector<Vector<double> > & design_jacobian, const Vector<Vector<double> > & beta) const
					{
						// Add diagonal terms
						const int nlevels = this->R.NumberOfRows();
						Vector<double> jac(nlevels * nvars);
						for (int index = 0, i = 0; i < nvars; ++i)
						{
							const Vector<double> & design_jacobian_i = design_jacobian(i);
							const Vector<double> & beta_i = beta(i);
							const double tau_i = this->tau(i);
							for (int k = 0; k < nlevels; ++k, ++index)
								jac(index) = design_jacobian_i(k) - tau_i * MatrixProduct(k, this->R, beta_i);
						}

						// Solve
						Vector<double> h_tmp = this->chol.Solve(jac);
						Vector<Vector<double> > h(this->nvars);
						for (int index = 0, i = 0; i < this->nvars; ++i)
						{
							h(i).Size(nlevels);
							Vector<double> & hi = h(i);
							for (int k = 0; k < nlevels; ++k, ++index)
								hi(k) = h_tmp(index);
						}
						return h;
					}
				}
			}
		}
	}
}
