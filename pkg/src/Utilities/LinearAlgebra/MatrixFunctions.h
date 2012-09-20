#ifndef UTILITIES_LINEARALGEBRA_MATRIXFUNCTIONS_H
#define UTILITIES_LINEARALGEBRA_MATRIXFUNCTIONS_H

#include "../Utilities.h"

namespace Utilities 
{
	namespace LinearAlgebra
	{
		// Operators
		Matrix<double> & operator *=(Matrix<double> & A, double a);
		Matrix<double> & operator +=(Matrix<double> & A, const ImmutableMatrix<double> & B);
		Matrix<double> & operator -=(Matrix<double> & A, const ImmutableMatrix<double> & B);
		Matrix<double> operator *(double a, const ImmutableMatrix<double> & A);
		Vector<double> operator *(const ImmutableMatrix<double> & A, const ImmutableVector<double> & v);
		Matrix<double> operator *(const ImmutableMatrix<double> & A, const ImmutableMatrix<double> & B);
		Matrix<double> operator +(const ImmutableMatrix<double> & A, const ImmutableMatrix<double> & B);
		Matrix<double> operator -(const ImmutableMatrix<double> & A, const ImmutableMatrix<double> & B);

		// Functions
		Matrix<double> Transpose(const ImmutableMatrix<double> & A);
		Matrix<double> Identity(int n);
		Matrix<double> Diagonal(const ImmutableVector<double> & v);
		Vector<double> Diagonal(const ImmutableMatrix<double> & A);
		Vector<double> RowSums(const ImmutableMatrix<double> & A);
		Vector<double> ColumnSums(const ImmutableMatrix<double> & A);
		double MaxAbs(const ImmutableMatrix<double> & A);
		double Trace(const ImmutableMatrix<double> & A);

		// Conversions
		TriangularMatrix<double> Lower(const ImmutableMatrix<double> & A);
		TriangularMatrix<double> Upper(const ImmutableMatrix<double> & A);
		TriangularMatrix<double> Symmetric(const ImmutableMatrix<double> & A);
	}
}

#endif
