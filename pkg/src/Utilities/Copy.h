#ifndef UTILITIES_COPY_H
#define UTILITIES_COPY_H

#include "Exceptions/Assertions.h"
#include "Vector.h"
#include "Matrix.h"
#include "TriangularMatrix.h"

namespace Utilities
{

	// Helper functions

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

	// Set and Copy functions

	// Assign constant to vector
	template <class TYPE>
	void Set(Vector<TYPE> & dst, const TYPE & x)
	{
		const int n = dst.Size();
		for (int i = 0; i < n; ++i)
			dst(i) = x;
	}

	// Copy vector to vector
	template <class TYPE>
	void Copy(Vector<TYPE> & dst, const Vector<TYPE> & src)
	{
		_VALIDATE_ARGUMENT(dst.Size() == src.Size());
		for (int i = 0; i < dst.Size(); ++i)
			dst(i) = src(i);
	}


	// Copy matrix to matrix
	template <class TYPE>
	void Copy(Matrix<TYPE> & dst, const Matrix<TYPE> & src)
	{
		_VALIDATE_ARGUMENT(dst.NumberOfRows() == src.NumberOfRows() &&
				dst.NumberOfColumns() == src.NumberOfColumns());
		for (int i = 0; i < dst.NumberOfRows(); ++i)
			for (int j = 0; j < dst.NumberOfColumns(); ++j)
				dst(i, j) = src(i, j);
	}

	// Copy vector to raw vector
	template <class TYPE>
	void Copy(TYPE * dst, int size, const Vector<TYPE> & src)
	{
		_VALIDATE_ARGUMENT(size == src.Size());
		for (int i = 0; i < size; ++i, ++dst)
			*dst = src(i);
	}

	// Copy matrix to raw vector
	template <class TYPE>
	void Copy(TYPE * dst, int size, const TriangularMatrix<TYPE> & src)
	{
		_VALIDATE_ARGUMENT(size == TotalSizeAsSquareMatrix(src));
		for (int i = 0; i < src.NumberOfRows(); ++i)
		{
			for (int j = 0; j < i; ++j, ++dst)
				*dst = src(i, j);
			for (int j = i; j < src.NumberOfRows(); ++j, ++dst)
				*dst = src(j, i);
		}
	}

	// Copy stratified vector to raw vector
	template <class TYPE>
	void Copy(TYPE * dst, int size, const Vector<Vector<TYPE> > & src)
	{
		_VALIDATE_ARGUMENT(size == TotalSize(src));
		for (int i = 0; i < src.Size(); ++i)
			for (int k = 0; k < src(i).Size(); ++k, ++dst)
				*dst = src(i)(k);
	}

	// Copy stratified matrix to raw vector
	template <class TYPE>
	void Copy(TYPE * dst, int size, const TriangularMatrix<Vector<TYPE> > & src)
	{
		_VALIDATE_ARGUMENT(size == TotalSizeAsStratifiedSquareMatrix(src));
		for (int i = 0; i < src.NumberOfRows(); ++i)
		{
			for (int j  = 0; j < i; ++j)
			{
				for (int k = 0; k < src(i, j).Size(); ++k, ++dst)
					*dst = src(i, j)(k);
			}
			for (int j  = i; j < src.NumberOfRows(); ++j)
			{
				for (int k = 0; k < src(j, i).Size(); ++k, ++dst)
					*dst = src(j, i)(k);
			}
		}
	}
}

#endif
