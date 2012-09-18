#include "../Utilities.h"
#include "SparseMatrix.h"
#include "SparseMatrixFunctions.h"

namespace Utilities 
{
	namespace LDL
	{
		Vector<double> ColumnSums(const SparseMatrix<double> & A)
		{
			const int ncols = A.NumberOfColumns();
			Vector<double> sums(ncols);
			for (int j = 0; j < ncols; ++j)
			{
				const int p2 = A.Count(j + 1);
				for (int p = A.Count(j); p < p2; ++p)
					sums(j) += A.Value(p);
			}
			return sums;
		}
	}
}
