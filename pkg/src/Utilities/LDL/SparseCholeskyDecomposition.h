#ifndef UTILITIES_LDL_SPARSECHOLESKYDECOMPOSITION_H
#define UTILITIES_LDL_SPARSECHOLESKYDECOMPOSITION_H

#include "../Utilities.h"
#include "SparseMatrix.h"

namespace Utilities 
{
	namespace LDL
	{
		// SparseCholeskyDecomposition
		class SparseCholeskyDecomposition
		{
		private:
			SparseMatrix<double> lower;
			Vector<double> diagonal;
			Vector<int> permutation;
			Vector<int> permutation_inverse;

		public:
			// Construction
			SparseCholeskyDecomposition();

			// Properties
			const SparseMatrix<double> Lower() const;
			const Vector<double> Diagonal() const;

			// Methods
			void Decompose(const SparseMatrix<double> upper);
			Vector<double> Solve(const Vector<double> b) const;
			//TriangularMatrix<double> Inverse() const;
			//double Determinant() const;
		};

		inline
		const SparseMatrix<double> SparseCholeskyDecomposition::Lower() const
		{
			return this->lower;
		}

		inline
		const Vector<double> SparseCholeskyDecomposition::Diagonal() const
		{
			return this->diagonal;
		}
	}
}

#endif
