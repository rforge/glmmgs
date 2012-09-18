#ifndef UTILITIES_SIZE_H
#define UTILITIES_SIZE_H

#include "Exceptions/Assertions.h"
#include "Vector.h"
#include "Matrix.h"
#include "TriangularMatrix.h"

namespace Utilities
{
	// Size of a stratified vector
	template <class TYPE>
	int TotalSize(const Vector<Vector<TYPE> > & v)
	{
		const int n = v.Size();
		int size = 0;
		for (int i = 0; i < n; ++i)
			size += v(i).Size();
		return size;
	}

	// Size of a triangular matrix
	template <class TYPE>
	int TotalSizeAsSquareMatrix(const TriangularMatrix<TYPE> & mat)
	{
		// Return size as it were a square matrix
		return mat.NumberOfRows() * mat.NumberOfRows();
	}

	// Size of a stratified triangular matrix
	template <class TYPE>
	int TotalSizeAsStratifiedSquareMatrix(const TriangularMatrix<Vector<TYPE> > & mat)
	{
		// Return size as it were a square matrix
		const int nvars = mat.NumberOfRows();
		int size = 0;
		for (int i = 0; i < nvars; ++i)
		{
			size += mat(i, i).Size();
			for (int j = 0; j < i; ++j)
				size += 2 * mat(i, j).Size();
		}
		return size;
	}
}

#endif
