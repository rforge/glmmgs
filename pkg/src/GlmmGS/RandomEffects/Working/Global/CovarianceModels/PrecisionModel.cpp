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
					double Trace(const Matrix<double> & a)
					{
						const int size = a.NumberOfColumns();
						double sum = 0.0;
						for (int i = 0; i < size; ++i)
							sum += a(i, i);
						return sum;
					}

					// Trace of square product
					double SquareTrace(const Matrix<double> & a)
					{
						const int size = a.NumberOfColumns();
						double sum = 0.0;
						for (int i = 0; i < size; ++i)
							for (int j = 0; j < size; ++j)
								sum += a(i, j) * a(j, i);
						return sum;
					}

					// Matrix-vector product
					double MatrixProduct(int k, WeakMatrix<const double> m, const Vector<double> & x)
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
						estimates(0) = Estimate(this->tau, 0.0); // TODO: calculate variance
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
						// Calulate T^{-1} R
						const int ncols = this->R.NumberOfColumns();
						Matrix<double> a(ncols, ncols);
						Vector<double> b(ncols);
						for (int j = 0; j < ncols; ++j)
						{
							// Prepare b
							for (int i = 0; i < ncols; ++i)
								b(i) = this->R(i, j);

							// Solve T x = b
							Vector<double> x = this->chol.Solve(b);

							// Store x
							for (int i = 0; i < ncols; ++i)
								a(i, j) = x(i);
						}

						// Calculate jacobian and minus the hessian
						const double bsquare = Square(this->R, beta);
						const double trace = Trace(a);
						const double jac = ncols / this->tau - bsquare - trace;
						const double minus_hessian = ncols / Math::Square(this->tau) - SquareTrace(a);

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
							jac(i) = design_jacobian(i) - this->tau * MatrixProduct(i, this->R, beta);

						// Solve
						return this->chol.Solve(jac);
					}
				}
			}
		}
	}
}
