#include "../../../../Standard.h"
#include "../../../../Estimate.h"
#include "PrecisionModel.h"

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
					// Helpers

					// Weighted square norm of vector
					double Square(WeakMatrix<const double> m, const Vector<double> & x)
					{
						_ASSERT_ARGUMENT(m.NumberOfRows() == x.Size() && m.NumberOfColumns() == x.Size());
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

					// Trace
					double Trace(const TriangularMatrix<double> & v, WeakMatrix<const double> m)
					{
						const int size = m.NumberOfRows();
						double sum = 0.0;
						for (int i = 0; i < size; ++i)
						{
							sum += v(i, i) * m(i, i);
							for (int j = 0; j < i; ++j)
								sum += 2.0 * v(i, j) * m(i, j);
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
						_ASSERT_ARGUMENT(offset_col < offset_row);
						const int size = m.NumberOfRows();
						double sum = 0.0;
						for (int i = 0; i < size; ++i)
							sum += v(offset_row + row, offset_col + i) * m(i, col);
						return sum;
					}

					// Matrix-matrix product
					double Product(const int i, const int j, const TriangularMatrix<double> & v, WeakMatrix<const double> m)
					{
						const int size = m.NumberOfRows();
						double sum = 0.0;
						for (int k = 0; k <= i; ++k)
							sum += v(i, k) * m(k, j);
						for (int k = i + 1; k < size; ++k)
							sum += v(k, i) * m(k, j);
						return sum;
					}

					// Trace of square product
					double SquareTrace(const TriangularMatrix<double> & v, WeakMatrix<const double> m)
					{
						const int size = m.NumberOfRows();
						double sum = 0.0;
						for (int i = 0; i < size; ++i)
						{
							sum += Math::Square(Product(i, i, v, m));
							for (int j = 0; j < i; ++j)
								sum += 2.0 * Product(i, j, v, m) * Product(j, i, v, m);
						}
						return sum;
					}

					// Matrix-vector product
					double Product(int k, WeakMatrix<const double> m, const Vector<double> & x)
					{
						const int size = m.NumberOfRows();
						double sum = 0.0;
						for (int i = 0; i < size; ++i)
							sum += m(k, i) * x(i);
						return sum;
					}

					// Construction
					PrecisionModel::PrecisionModel(WeakMatrix<const double> R)
						: R(R), tau(1.0)
					{
					}

					PrecisionModel::~PrecisionModel()
					{
					}

					// Properties
					Vector<Estimate> PrecisionModel::Estimates() const
					{
						Vector<Estimate> estimates(1);
						estimates(0) = Estimate(1.0 / sqrt(this->tau), -1); // TODO: calculate variance
						return estimates;
					}

					// Methods
					void PrecisionModel::Decompose(const TriangularMatrix<double> & design_precision)
					{
						// Add diagonal to precision
						TriangularMatrix<double> prec = design_precision;
						const int size = this->R.NumberOfRows();
						for (int i = 0; i < size; ++i)
						{
							prec(i, i) += this->tau * this->R(i, i);
							for (int j = 0; j < i; ++j)
								prec(i, j) += this->R(i, j);
						}

						// Decompose
						this->chol.Decompose(prec);
					}

					int PrecisionModel::Update(const Vector<double> & beta, Comparer comparer)
					{
						// Calculate variance
						const TriangularMatrix<double> variance = this->chol.Inverse();

						// Calculate jacobian and minus the hessian
						const int size = this->R.NumberOfRows();
						const double bsquare = Square(this->R, beta);
						const double trace = Trace(variance, this->R);
						const double jac = size / this->tau - bsquare - trace;
						const double minus_hessian = size / Math::Square(this->tau) - SquareTrace(variance, this->R);

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

						return update;
					}

					Vector<double> PrecisionModel::CoefficientsUpdate(const Vector<double> & design_jacobian, const Vector<double> & beta) const
					{
						// Add diagonal terms
						const int size = this->R.NumberOfRows();
						Vector<double> jac(size);
						for (int i = 0; i < size; ++i)
							jac(i) = design_jacobian(i) - this->tau * Product(i, this->R, beta);

						// Solve
						return this->chol.Solve(jac);
					}
				}
			}
		}
	}
}
