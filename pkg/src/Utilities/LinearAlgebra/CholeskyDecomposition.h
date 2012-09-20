#ifndef UTILITIES_LINEARALGEBRA_CHOLESKYDECOMPOSITION_H
#define UTILITIES_LINEARALGEBRA_CHOLESKYDECOMPOSITION_H

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
			explicit CholeskyDecomposition(const ImmutableTriangularMatrix<double> & A);

			// Properties
			const ImmutableTriangularMatrix<double> & Lower() const;

			// Methods
			void Decompose(const ImmutableTriangularMatrix<double> & A);
			Vector<double> Solve(const ImmutableVector<double> & b) const;
			TriangularMatrix<double> Inverse() const;
			double Determinant() const;
		};

		// Properties
		inline
		const ImmutableTriangularMatrix<double> & CholeskyDecomposition::Lower() const
		{
			return this->lower;
		}
	}
}

#endif
