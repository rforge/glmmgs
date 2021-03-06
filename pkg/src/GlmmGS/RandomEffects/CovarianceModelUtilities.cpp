#include "../Standard.h"

namespace GlmmGS
{
	namespace RandomEffects
	{

		void ValidateTheta(const ImmutableVector<double> & theta, const ImmutableVector<double> & init_theta)
		{
			if (theta.Size() != init_theta.Size())
				throw InvalidArgumentException("Wrong number of variance components in \'init_theta\'");
		}

		// Weighted squared norm of a vector - m assumed symmetric
		double Square(const ImmutableMatrix<double> & m, const ImmutableVector<double> & x)
		{
			_ASSERT(m.NumberOfRows() == x.Size() && m.NumberOfColumns() == x.Size());
			const int size = x.Size();
			double sum = 0.0;
			for (int i = 0; i < size; ++i)
			{
				// Diagonal term
				sum += m(i, i) * Math::Square(x(i));

				// Off diagonal terms
				double tmp = 0.0;
				for (int j = 0; j < i; ++j)
					tmp += m(i, j) * x(j);
				sum += 2.0 * x(i) * tmp;
			}
			return sum;
		}

		// Weighted squared norm of a vector
		double Square(const LDL::SparseMatrix<double> & R, const ImmutableVector<double> & x)
		{
			_ASSERT(R.NumberOfColumns() == x.Size());
			const int ncols = R.NumberOfColumns();
			double sum = 0.0;
			for (int j = 0; j < ncols; ++j)
			{
				const int p2 = R.Count(j + 1);
				double tmp = 0.0;
				for (int p = R.Count(j); p < p2; ++p)
				{
					const int i = R.Index(p);
					const double value = R.Value(p);
					tmp += x(i) * value;
				}
				sum += x(j) * tmp;
			}
			return sum;
		}

		// Trace of diagonal blocks
		double BlockTrace(int row, int nlevels, const ImmutableMatrix<double> & a)
		{
			const int offset = row * nlevels;
			double sum = 0.0;
			for (int j = 0; j < nlevels; ++j)
				sum += a(offset + j, offset + j);
			return sum;
		}

		// Trace of diagonal blocks product
		double BlockSquareTrace(int row, int col, int nlevels, const ImmutableMatrix<double> & a)
		{
			const int offset_row = row * nlevels;
			const int offset_col = col * nlevels;
			double sum = 0.0;
			for (int i = 0; i < nlevels; ++i)
				for (int j = 0; j < nlevels; ++j)
					sum += a(offset_row + i, offset_col + j) * a(offset_col + j, offset_row + i);
			return sum;
		}


		// Matrix product
		double MatrixProduct(int k, const ImmutableMatrix<double> & m, const ImmutableVector<double> & x)
		{
			const int size = m.NumberOfRows();
			double sum = 0.0;
			for (int i = 0; i < size; ++i)
				sum += m(k, i) * x(i);
			return sum;
		}

		// Transposed-matrix product
		double TMatrixProduct(int k, const LDL::SparseMatrix<double> & R, const ImmutableVector<double> & x)
		{
			double sum = 0.0;
			const int p2 = R.Count(k + 1);
			for (int p = R.Count(k); p < p2; ++p)
			{
				const int i = R.Index(p);
				const double value = R.Value(p);
				sum += value * x(i);
			}
			return sum;
		}
	}
}
