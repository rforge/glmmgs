#include "../../../../Standard.h"
#include "../../../../Estimate.h"
#include "PrecisionModel.h"

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
					// Helpers

					// Weighted square norm of vector
					double Square(WeakMatrix<const double> m, const Vector<double> & x)
					{
						GLMMGS_ASSERT_ARGUMENT(m.NumberOfRows() == x.Size() && m.NumberOfColumns() == x.Size());
						const int size = x.Size();
						double sum = 0.0;
						for (int i = 0; i < size; ++i)
						{
							sum += m(i, i) * Math::Square(x(i));
							for (int j = 0; j < i; ++j)
								sum += 2.0 * m(i, j) * x(i) * x(j);
						}
						return sum;
					}

					// Trace of diagonal blocks
					double BlockTrace(int row, const TriangularMatrix<double> & v, WeakMatrix<const double> m)
					{
						const int size = m.NumberOfRows();
						const int offset = row * size;
						double sum = 0.0;
						for (int i = 0; i < size; ++i)
						{
							sum += v(offset + i, offset + i) * m(i, i);
							for (int j = 0; j < i; ++j)
								sum += 2.0 * v(offset + i, offset + j) * m(i, j);
						}
						return sum;
					}

					// Diagonal block product
					double BlockProduct(int row, int col, const TriangularMatrix<double> & v, int offset, WeakMatrix<const double> m)
					{
						const int size = m.NumberOfRows();
						double sum = 0.0;
						for (int i = 0; i <= row; ++i)
							sum += v(offset + row, offset + i) * m(i, col);
						for (int i = row + 1; i < size; ++i)
							sum += v(offset + i, offset + row) * m(i, col);
						return sum;
					}

					// Off-diagonal block product
					double BlockProduct(int row, int col, const TriangularMatrix<double> & v, int offset_row, int offset_col, WeakMatrix<const double> m)
					{
						GLMMGS_ASSERT_ARGUMENT(offset_col < offset_row);
						const int size = m.NumberOfRows();
						double sum = 0.0;
						for (int i = 0; i < size; ++i)
							sum += v(offset_row + row, offset_col + i) * m(i, col);
						return sum;
					}

					// Trace of diagonal blocks product
					double BlockSquareTrace(int row, const TriangularMatrix<double> & v, WeakMatrix<const double> m)
					{
						const int size = m.NumberOfRows();
						const int offset = row * size;
						double sum = 0.0;
						for (int i = 0; i < size; ++i)
							for (int j = 0; j < size; ++j)
								sum += BlockProduct(i, j, v, offset, m) * BlockProduct(j, i, v, offset, m);
						return sum;
					}

					// Trace of off-diagonal blocks product
					double BlockSquareTrace(int row, int col, const TriangularMatrix<double> & v, WeakMatrix<const double> m)
					{
						GLMMGS_ASSERT_ARGUMENT(col < row);
						const int size = m.NumberOfRows();
						const int offset_row = row * size;
						const int offset_col = col * size;
						double sum = 0.0;
						for (int i = 0; i < size; ++i)
							for (int j = 0; j < size; ++j)
								sum += BlockProduct(i, j, v, offset_row, offset_col, m) * BlockProduct(j, i, v, offset_row, offset_col, m);
						return sum;
					}

					// Matrix product
					double MatrixProduct(int k, WeakMatrix<const double> m, const Vector<double> & x)
					{
						const int size = m.NumberOfRows();
						double sum = 0.0;
						for (int i = 0; i < size; ++i)
							sum += m(k, i) * x(i);
						return sum;
					}

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
							estimates(j) = Estimate(1.0 / sqrt(this->tau(j)), -1); // TODO: calculate variance
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
						// Calculate variance
						const TriangularMatrix<double> variance = this->chol.Inverse();

						// Calculate jacobian and minus the hessian
						Vector<double> jac(this->nvars);
						TriangularMatrix<double> minus_hessian(this->nvars);
						const int nlevels = this->R.NumberOfRows();
						for (int i = 0; i < this->nvars; ++i)
						{
							const double bsquare = Square(this->R, beta(i));
							const double trace = BlockTrace(i, variance, this->R);
							jac(i) = nlevels / this->tau(i) - bsquare - trace;
							minus_hessian(i, i) = nlevels / Math::Square(this->tau(i)) - BlockSquareTrace(i, variance, this->R);
							for (int j = 0; j < i; ++j)
								minus_hessian(i, j) = -BlockSquareTrace(i, j, variance, this->R);
						}
						// Calculate update
						CholeskyDecomposition chol(minus_hessian);
						Vector<double> h = chol.Solve(jac);

						const int update = comparer.IsZero(h, this->tau) ? 0 : 1;

						// Update sigma_square
						this->tau += h;
						while (Min(this->tau) <= 0.0)
						{
							// Back-track
							h *= 0.5;
							this->tau -= h;
						}

						// Debug
						Print("Update covariance components= %g\n", h);
						//Print("Covariance components = %g\n", this->tau);
						//Print("Update covariance components: %d\n", update);

						return update;
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
								jac(index) = design_jacobian_i(k) - tau_i * CovarianceModels::MatrixProduct(k, this->R, beta_i);
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
