#ifndef UTILITIES_LINEARALGEBRA_TRIANGULARMATRIXFUNCTIONS_H
#define UTILITIES_LINEARALGEBRA_TRIANGULARMATRIXFUNCTIONS_H

#include "../Utilities.h"

namespace Utilities 
{
	namespace LinearAlgebra
	{
		// Operators
		TriangularMatrix<double> & operator *=(TriangularMatrix<double> & A, double a);
		TriangularMatrix<double> & operator +=(TriangularMatrix<double> & A, const ImmutableTriangularMatrix<double> & B);
		TriangularMatrix<double> & operator -=(TriangularMatrix<double> & A, const ImmutableTriangularMatrix<double> & B);
		TriangularMatrix<double> operator +(TriangularMatrix<double> & A, const ImmutableTriangularMatrix<double> & B);
		TriangularMatrix<double> operator -(TriangularMatrix<double> & A, const ImmutableTriangularMatrix<double> & B);
		TriangularMatrix<double> operator *(double a, const ImmutableTriangularMatrix<double> & A);

		// Functions
		Vector<double> Diagonal(const ImmutableTriangularMatrix<double> & A);
		double Trace(const ImmutableTriangularMatrix<double> & A);
		double SquareTrace(const ImmutableTriangularMatrix<double> & A);

		// Conversion
		Matrix<double> ToLower(const ImmutableTriangularMatrix<double> & A);
		Matrix<double> ToUpper(const ImmutableTriangularMatrix<double> & A);
		Matrix<double> ToSymmetric(const ImmutableTriangularMatrix<double> & A);

		// Vectorial functions
		int TotalSize(const ImmutableTriangularMatrix<Vector<double> > & x);
	}
}

#endif
