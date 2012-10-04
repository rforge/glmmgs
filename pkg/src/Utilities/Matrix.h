#ifndef UTILITIES_ImmutableMatrix_H
#define UTILITIES_ImmutableMatrix_H

#include "Exceptions/Assertions.h"
#include "External.h"
#include "Array.h"

namespace Utilities
{
	// ImmutableMatrix
	template <class TYPE>
	class ImmutableMatrix
	{
	protected:
		// Fields
		Array<TYPE> array;
		int nrows;
		int ncols;

	public:
		// Construction
		ImmutableMatrix();
		ImmutableMatrix(int nrows, int ncols);
		ImmutableMatrix(External<TYPE> ext, int nrows, int ncols);

		// Properties
		int NumberOfRows() const;
		int NumberOfColumns() const;

		// Element access
		const typename Immutable<TYPE>::Type & operator ()(int i, int j) const;
	};


	// Definition

	// Construction
	template <class TYPE> inline
	ImmutableMatrix<TYPE>::ImmutableMatrix()
		: nrows(0), ncols(0)
	{
	}

	template <class TYPE> inline
	ImmutableMatrix<TYPE>::ImmutableMatrix(int nrows, int ncols)
		: array(nrows * ncols), nrows(nrows), ncols(ncols)
	{
	}

	template <class TYPE> inline
	ImmutableMatrix<TYPE>::ImmutableMatrix(External<TYPE> ext, int nrows, int ncols)
		: array(ext, nrows * ncols), nrows(nrows), ncols(ncols)
	{
	}

	// Properties
	template <class TYPE> inline
	int ImmutableMatrix<TYPE>::NumberOfRows() const
	{
		return this->nrows;
	}

	template <class TYPE> inline
	int ImmutableMatrix<TYPE>::NumberOfColumns() const
	{
		return this->ncols;
	}

	// Element access
	template <class TYPE> inline
	const typename Immutable<TYPE>::Type & ImmutableMatrix<TYPE>::operator ()(int i, int j) const
	{
		_ASSERT(i >= 0 && i < this->nrows && j >= 0 && j < this->ncols);
		return this->array[i * this->ncols + j];
	}

	// Matrix
	template <class TYPE>
	class Matrix : public ImmutableMatrix<TYPE>
	{
	public:
		// Construction
		Matrix();
		Matrix(int nrows, int ncols);
		Matrix(External<TYPE> ext, int nrows, int ncols);

		// Element access
		TYPE & operator ()(int i, int j);
		const typename Immutable<TYPE>::Type & operator ()(int i, int j) const;
	};


	// Definition

	// Construction
	template <class TYPE> inline
	Matrix<TYPE>::Matrix()
	{
	}

	template <class TYPE> inline
	Matrix<TYPE>::Matrix(int nrows, int ncols)
		: ImmutableMatrix<TYPE>(nrows, ncols)
	{
	}

	template <class TYPE> inline
	Matrix<TYPE>::Matrix(External<TYPE> ext, int nrows, int ncols)
		: ImmutableMatrix<TYPE>(ext, nrows, ncols)
	{
	}

	// Element access
	template <class TYPE> inline
	TYPE & Matrix<TYPE>::operator ()(int i, int j)
	{
		_ASSERT(i >= 0 && i < this->nrows && j >= 0 && j < this->ncols);
		return this->array[i * this->ncols + j];
	}

	template <class TYPE> inline
	const typename Immutable<TYPE>::Type & Matrix<TYPE>::operator ()(int i, int j) const
	{
		_ASSERT(i >= 0 && i < this->nrows && j >= 0 && j < this->ncols);
		return this->array[i * this->ncols + j];
	}

	// Specialize Immutable class
	template <class TYPE>
	struct Immutable<Matrix<TYPE> >
	{
		typedef ImmutableMatrix<TYPE> Type;
	};
}

#endif
