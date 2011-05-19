#pragma once

#include "../Utilities.h"

namespace Utilities 
{
	namespace LinearAlgebra
	{
		// CholeskyDecomposition
		class CholeskyDecomposition
		{
		private:
			// Fields
			TriangularMatrix<double> lower;
		
		public:
			// Construction
			CholeskyDecomposition();
			explicit CholeskyDecomposition(const TriangularMatrix<double> & A);

			// Methods
			void Decompose(const TriangularMatrix<double> & A);
			Vector<double> Solve(const Vector<double> & b) const;
			TriangularMatrix<double> Inverse() const;
			double Determinant() const;

			// Properties
			int NumberOfColumns() const;
			int NumberOfRows() const;
			const TriangularMatrix<double> & L() const;
			double L(int i, int j) const;
		};

		// Definition
		inline
		int CholeskyDecomposition::NumberOfColumns() const
		{
			return this->lower.NumberOfColumns();
		}

		inline
		int CholeskyDecomposition::NumberOfRows() const
		{
			return this->lower.NumberOfRows();
		}

		inline
		const TriangularMatrix<double> & CholeskyDecomposition::L() const
		{
			return this->lower;
		}
		
		inline
		double CholeskyDecomposition::L(int i, int j) const
		{
			return this->lower(i, j);
		}
	}
}
