#include "../Utilities.h"
#include "../Exceptions/NumericException.h"
#include "../Math/Math.h"
#include "CholeskyDecomposition.h"

namespace Utilities 
{
	namespace LinearAlgebra
	{
		CholeskyDecomposition::CholeskyDecomposition()
		{
		}

		CholeskyDecomposition::CholeskyDecomposition(const ImmutableTriangularMatrix<double> & A)
		{
			this->Decompose(A);
		}
		
		void CholeskyDecomposition::Decompose(const ImmutableTriangularMatrix<double> & A)
		{
			const int n = A.NumberOfRows();
			this->lower = TriangularMatrix<double>(n);
			
			// Computes lower triangular Matrix such that L * Transpose(L) = A
			for (int i = 0; i < n; ++i)
			{
				// j == i
				{
					double sum = A(i, i);
					for (int k = 0; k < i; ++k)
						sum -= Math::Square(this->lower(i, k));
					if (sum <= 0.0)
						throw Exceptions::NumericException("Non positive matrix in Cholesky decomposition");
					this->lower(i, i) = sqrt(sum);
				}

				// j > i
				for (int j = i + 1; j < n; ++j)
				{
					double sum = A(j, i);
					for (int k = 0; k < i; ++k)
						sum -= this->lower(i, k) * this->lower(j, k);
					this->lower(j, i) = sum / this->lower(i, i);
				}
			}
		}
		
		Vector<double> CholeskyDecomposition::Solve(const ImmutableVector<double> & b) const
		{
			_ASSERT(b.Size() == this->lower.NumberOfRows());
			const int n = this->lower.NumberOfRows();
			Vector<double> x(n);
			
			// Back-substitution algorithm
			for (int i = 0; i < n; ++i)
			{
				double sum = b(i);
				for (int k = i - 1; k >= 0; --k)
					sum -= this->lower(i, k) * x(k);
				x(i) = sum / this->lower(i, i);
			}
			for (int i = n - 1; i >= 0; --i)
			{
				double sum = x(i);
				for (int k = i + 1; k < n; ++k)
					sum -= this->lower(k, i) * x(k);
				x(i) = sum / this->lower(i, i);
			}
			return x;
		}

		TriangularMatrix<double> CholeskyDecomposition::Inverse() const
		{
			const int n = this->lower.NumberOfRows();
			TriangularMatrix<double> X(n);

			// Calculate inverse
			for (int j = 0; j < n; ++j)
			{
				// Back-substitution algorithm with b = delta(j)
				for (int i = j; i < n; ++i)
				{
					double sum = ((i == j) ? 1.0 : 0.0);
					for (int k = i - 1; k >= j; --k)
						sum -= this->lower(i, k) * X(k, j);
					X(i, j) = sum / this->lower(i, i);
				}

				for (int i = n - 1; i >= j; --i)
				{
					double sum = X(i, j);
					for (int k = i + 1; k < n; ++k)
						sum -= this->lower(k, i) * X(k, j);
					X(i, j) = sum / this->lower(i, i);
				}
			}
			return X;
		}

		double CholeskyDecomposition::Determinant() const
		{
			const int n = this->lower.NumberOfRows();
			double det = 1.0;
			for (int j = 0; j < n; ++j)
				det *= this->lower(j, j);
			return det * det;
		}
	}
}
