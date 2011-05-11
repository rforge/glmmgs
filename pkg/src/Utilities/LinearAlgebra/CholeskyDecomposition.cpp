#include "CholeskyDecomposition.h"
#include "../Exceptions/NumericException.h"
#include "../Math/Math.h"

namespace Utilities 
{
	namespace LinearAlgebra
	{
		const CholeskyDecomposition::DecomposeInverse CholeskyDecomposition::decompose_inverse = CholeskyDecomposition::DecomposeInverse();

		CholeskyDecomposition::CholeskyDecomposition()
		{
		}

		CholeskyDecomposition::CholeskyDecomposition(const TriangularMatrix<double> & A)
		{
			this->Decompose(A);
		}
		
		CholeskyDecomposition::CholeskyDecomposition(const TriangularMatrix<double> & A, DecomposeInverse)
		{
			this->Decompose(A, CholeskyDecomposition::decompose_inverse);
		}
		
		void CholeskyDecomposition::Decompose(const TriangularMatrix<double> & A)
		{
			this->lower = A;
			const int n = this->lower.NumberOfRows();
			
			// Computes lower triangular Matrix such that L * Transpose(L) = A
			for (int i = 0; i < n; ++i)
			{
				// j == i
				{
					double sum = this->lower(i, i);
					for (int k = i - 1; k >= 0; --k) 
						sum -= Math::Square(this->lower(i, k));
					if (sum <= 0.0)
						throw Exceptions::NumericException("Non positive matrix in Cholesky decomposition");
					this->lower(i, i) = sqrt(sum);
				}

				// j > i
				for (int j = i + 1; j < n; ++j)
				{
					double sum = this->lower(j, i);
					for (int k = i - 1; k >= 0; --k) 
						sum -= this->lower(i, k) * this->lower(j, k);
					this->lower(j, i) = sum / this->lower(i, i);
				}
			}
		}

		void CholeskyDecomposition::Decompose(const TriangularMatrix<double> & A, DecomposeInverse)
		{
			// Compute lower triangular Matrix such that L * L^T = A^{-1}
			
			this->lower = A;
			int n = this->lower.NumberOfRows();
			
			// Compute upper triangular Matrix such that U * U^T = A
			for (int i = n - 1; i >= 0; --i)
			{
				// j > i
				for (int j = n - 1; j > i; --j)
				{
					double sum = this->lower(j, i);
					for (int k = j + 1; k < n; ++k) 
						sum -= this->lower(k, i) * this->lower(k, j);
					this->lower(j, i) = sum / this->lower(j, j);
				}

				// j == i
				{
					double sum = this->lower(i, i);
					for (int k = i + 1; k < n; ++k) 
						sum -= Math::Square(this->lower(k, i));
					if (sum <= 0.0)
						throw Exceptions::NumericException("Non positive matrix in Cholesky decomposition");
					this->lower(i, i) = sqrt(sum);
				}
			}

			// Compute L = (U^T)^{-1}
			for (int i = n - 1; i >= 0; --i)
			{
				// j > i
				for (int j = n - 1; j > i; --j)
				{
					double sum = 0.0;
					for (int k = i + 1; k <= j; ++k)
						sum += this->lower(k, i) * this->lower(j, k);
					this->lower(j, i) = -sum / this->lower(i, i);
				}

				// j == i
				this->lower(i, i) = 1.0 / this->lower(i, i);
			}
		}
		
		Vector<double> CholeskyDecomposition::Solve(const Vector<double> & b) const
		{
			GLMMGS_ASSERT_ARGUMENT(b.Size() == this->lower.NumberOfRows())
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
