#pragma once

#include "../Utilities.h"

namespace Utilities 
{
	namespace LinearAlgebra
	{
		// VectorialCholeskyDecomposition
		class VectorialCholeskyDecomposition
		{
		private:
			// Fields
			TriangularMatrix<Vector<double> > lower;
		
		public:
			// Construction
			VectorialCholeskyDecomposition();
			explicit VectorialCholeskyDecomposition(const TriangularMatrix<Vector<double> > & A);

			// Methods
			void Decompose(const TriangularMatrix<Vector<double> > & A);
			Vector<Vector<double> > Solve(const Vector<Vector<double> > & b) const;
			TriangularMatrix<Vector<double> > Inverse() const;
			double Determinant() const;

			// Properties
			int NumberOfColumns() const;
			int NumberOfRows() const;
			const TriangularMatrix<Vector<double> > & L() const;
			const Vector<double> & L(int i, int j) const;
		};

		// Definition
		inline
		int VectorialCholeskyDecomposition::NumberOfColumns() const
		{
			return this->lower.NumberOfColumns();
		}

		inline
		int VectorialCholeskyDecomposition::NumberOfRows() const
		{
			return this->lower.NumberOfRows();
		}

		inline
		const TriangularMatrix<Vector<double> > & VectorialCholeskyDecomposition::L() const
		{
			return this->lower;
		}
		
		inline
		const Vector<double> & VectorialCholeskyDecomposition::L(int i, int j) const
		{
			return this->lower(i, j);
		}
	}
}
