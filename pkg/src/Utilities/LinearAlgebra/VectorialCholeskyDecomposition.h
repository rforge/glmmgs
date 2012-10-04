#ifndef UTILITIES_LINEARALGEBRA_VECTORIALCHOLESKYDECOMPOSITION_H
#define UTILITIES_LINEARALGEBRA_VECTORIALCHOLESKYDECOMPOSITION_H

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
			explicit VectorialCholeskyDecomposition(const ImmutableTriangularMatrix<Vector<double> > & A);

			// Properties
			const ImmutableTriangularMatrix<Vector<double> > & Lower() const;

			// Methods
			void Decompose(const ImmutableTriangularMatrix<Vector<double> > & A);
			Vector<Vector<double> > Solve(const ImmutableVector<Vector<double> > & b) const;
			TriangularMatrix<Vector<double> > Inverse() const;
			double Determinant() const;
		};

		// Properties
		inline
		const ImmutableTriangularMatrix<Vector<double> > & VectorialCholeskyDecomposition::Lower() const
		{
			return this->lower;
		}
	}
}

#endif
