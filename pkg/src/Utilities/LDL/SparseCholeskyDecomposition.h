#pragma once

#include "../Utilities.h"
#include "../NewTypes/NewTypes.h"
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
			NewTypes::Vector<double> diagonal;

		public:
			// Construction
			SparseCholeskyDecomposition();

			// Properties
			const SparseMatrix<double> Lower() const;
			const NewTypes::Vector<double> Diagonal() const;

			// Methods
			void Decompose(const SparseMatrix<double> upper);
			NewTypes::Vector<double> Solve(const NewTypes::Vector<double> b) const;
			//NewTypes::TriangularMatrix<double> Inverse() const;
			//double Determinant() const;
		};

		inline
		const SparseMatrix<double> SparseCholeskyDecomposition::Lower() const
		{
			return this->lower;
		}

		inline
		const NewTypes::Vector<double> SparseCholeskyDecomposition::Diagonal() const
		{
			return this->diagonal;
		}
	}
}
