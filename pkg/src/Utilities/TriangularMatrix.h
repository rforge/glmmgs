#ifndef UTILITIES_TRIANGULARMATRIX_H
#define UTILITIES_TRIANGULARMATRIX_H

#include "Exceptions/Assertions.h"
#include "External.h"
#include "Array.h"

namespace Utilities
{
	// ImmutableTriangularMatrix
	template <class TYPE>
	class ImmutableTriangularMatrix
	{
	protected:
		// Fields
		Array<TYPE> array;
		int nrows;

		// Helper function returning the number of elements associated with a row
		static int Count(int row);

	public:
		// Construction
		ImmutableTriangularMatrix();
		explicit ImmutableTriangularMatrix(int nrows);
		ImmutableTriangularMatrix(External<TYPE> ptr, int nrows);

		// Attributes
		int NumberOfRows() const;

		// Element access
		const TYPE & operator ()(int i, int j) const;
	};

	// Construction
	template <class TYPE> inline
	ImmutableTriangularMatrix<TYPE>::ImmutableTriangularMatrix()
		: nrows(0)
	{
	}

	template <class TYPE> inline
	ImmutableTriangularMatrix<TYPE>::ImmutableTriangularMatrix(int nrows)
		: array(Count(nrows)), nrows(nrows)
	{
	}

	template <class TYPE> inline
	ImmutableTriangularMatrix<TYPE>::ImmutableTriangularMatrix(External<TYPE> ext, int nrows)
		: array(ext, Count(nrows)), nrows(nrows)
	{
	}

	// Properties
	template <class TYPE> inline
	int ImmutableTriangularMatrix<TYPE>::NumberOfRows() const
	{
		return this->nrows;
	}

	// Element access
	template <class TYPE> inline
	const TYPE & ImmutableTriangularMatrix<TYPE>::operator ()(int i, int j) const
	{
		_ASSERT(i >= 0 && i < this->nrows && j >= 0 && j <= i);
		return this->array[Count(i) + j];
	}

	// Helper
	template <class TYPE> inline
	int ImmutableTriangularMatrix<TYPE>::Count(int row)
	{
		return (row * (row + 1)) >> 1;
	}


	// TriangularMatrix
	template <class TYPE>
	class TriangularMatrix : public ImmutableTriangularMatrix<TYPE>
	{
	public:
		// Construction
		TriangularMatrix();
		explicit TriangularMatrix(int nrows);
		TriangularMatrix(External<TYPE> ext, int nrows);

		// Element access
		TYPE & operator ()(int i, int j);
		const TYPE & operator ()(int i, int j) const;
	};

	// Construction
	template <class TYPE> inline
	TriangularMatrix<TYPE>::TriangularMatrix()
	{
	}

	template <class TYPE> inline
	TriangularMatrix<TYPE>::TriangularMatrix(int nrows)
		: ImmutableTriangularMatrix<TYPE>(nrows)
	{
	}

	template <class TYPE> inline
	TriangularMatrix<TYPE>::TriangularMatrix(External<TYPE> ext, int nrows)
		: ImmutableTriangularMatrix<TYPE>(ext, nrows)
	{
	}

	// Element access
	template <class TYPE> inline
	TYPE & TriangularMatrix<TYPE>::operator ()(int i, int j)
	{
		_ASSERT(i >= 0 && i < this->nrows && j >= 0 && j <= i);
		return this->array[ImmutableTriangularMatrix<TYPE>::Count(i) + j];
	}

	template <class TYPE> inline
	const TYPE & TriangularMatrix<TYPE>::operator ()(int i, int j) const
	{
		_ASSERT(i >= 0 && i < this->nrows && j >= 0 && j <= i);
		return this->array[ImmutableTriangularMatrix<TYPE>::Count(i) + j];
	}
}

#endif
