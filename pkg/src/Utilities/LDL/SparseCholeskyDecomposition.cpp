#include "../Utilities.h"
#include "../Exceptions/Exceptions.h"
#include "../NewTypes/NewTypes.h"
#include "SparseCholeskyDecomposition.h"
#include "SparseMatrix.h"

#include "Internal/ldl.h"

namespace Utilities 
{
	namespace LDL
	{
		using namespace Utilities::NewTypes;

		SparseCholeskyDecomposition::SparseCholeskyDecomposition()
		{
		}

		void SparseCholeskyDecomposition::Decompose(const SparseMatrix<double> upper)
		{
			// Computes lower triangular matrix such that L D L^T = U

			const int ncols = upper.NumberOfColumns();

			// Symbolic decomposition
			NewTypes::Vector<int> lp(ncols + 1);
			Array<int> parent(ncols);
			Array<int> lnz(ncols);
			Array<int> flag(ncols);
			Array<int> null;

			Internal::LDL_symbolic(ncols,
					Cast<const Array<int> >(upper.Counts()),
					Cast<const Array<int> >(upper.Indices()),
					Cast<Array<int> >(lp),
					parent, lnz, flag, null, null);

			// Numeric decomposition
			const int lsize = lp(ncols);
			NewTypes::Vector<double> lx(lsize);
			NewTypes::Vector<int> li(lsize);
			NewTypes::Vector<double> d(ncols);
			Array<double> y(ncols);
			Array<int> pattern(ncols);

			Internal::LDL_numeric(ncols,
					Cast<const Array<int> >(upper.Counts()),
					Cast<const Array<int> >(upper.Indices()),
					Cast<const Array<double> >(upper.Values()),
					Cast<const Array<int> >(lp),
					parent, lnz,
					Cast<const Array<int> >(li),
					Cast<Array<double> >(lx),
					Cast<Array<double> >(d),
					y, pattern, flag, null, null);

			// TODO: check return value

			// Set members
			this->lower = SparseMatrix<double>(ncols, lx, li, lp);
			this->diagonal = d;
		}

		NewTypes::Vector<double> SparseCholeskyDecomposition::Solve(const NewTypes::Vector<double> b) const
		{
			_VALIDATE_ARGUMENT(b.Size() == this->lower.NumberOfColumns());

			const int ncols = this->lower.NumberOfColumns();
			NewTypes::Vector<double> x(ncols);
			CopyVector(x, b);
			Internal::LDL_lsolve(ncols,
					Cast<Array<double> >(x),
					Cast<const Array<int> >(this->lower.Counts()),
					Cast<const Array<int> >(this->lower.Indices()),
					Cast<const Array<double> >(this->lower.Values()));

			Internal::LDL_dsolve(ncols,
					Cast<Array<double> >(x),
					Cast<const Array<double> >(this->diagonal));

			Internal::LDL_ltsolve(ncols,
					Cast<Array<double> >(x),
					Cast<const Array<int> >(this->lower.Counts()),
					Cast<const Array<int> >(this->lower.Indices()),
					Cast<const Array<double> >(this->lower.Values()));

			return x;
		}
		/*
		TriangularMatrix<double> SparseCholeskyDecomposition::Inverse() const
		{
			const int n = this->lower.NumberOfRows();
			TriangularMatrix<double> x(n);

			// Calculate inverse
			for (int j = 0; j < n; ++j)
			{
				// Back-substitution algorithm with b = delta(j)
				// i == j
				{
					const double sum = 1.0 - SparseLinearAlgebra::ScalarProduct(this->lower.SubRow(j), x.Column(j));
					x(j, j) = sum / this->lower.Diagonal(j);
				}

				// i > j
				for (int i = j + 1; i < n; ++i)
				{
					const double sum = SparseLinearAlgebra::ScalarProduct(this->lower.SubRow(i), x.Column(j));
					x(i, j) = sum / this->lower.Diagonal(i);
				}

				for (int i = n - 1; i >= j; --i)
				{
					const double sum = x(i, j) - SparseLinearAlgebra::ScalarProduct(this->lower.SubColumn(i), x.Column(j));
					x(i, j) = sum / this->lower.Diagonal(i);
				}
			}
			return x;
		}

		double SparseCholeskyDecomposition::Determinant() const
		{
			const int n = this->lower.NumberOfRows();
			double det = 1.0;
			for (int j = 0; j < n; ++j)
				det *= this->lower.Diagonal(j);
			return det * det;
		}
		*/
	}
}
