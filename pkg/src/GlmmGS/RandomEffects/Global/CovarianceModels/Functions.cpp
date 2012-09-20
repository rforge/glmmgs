#include "../../../Standard.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Global
		{
			namespace CovarianceModels
			{
				// Weighted square norm of vector
				double Square(Matrix<const double> m, const ImmutableVector<double> & x)
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
				double MatrixProduct(int k, Matrix<const double> m, const ImmutableVector<double> & x)
				{
					const int size = m.NumberOfRows();
					double sum = 0.0;
					for (int i = 0; i < size; ++i)
						sum += m(k, i) * x(i);
					return sum;
				}
			}
		}
	}
}
