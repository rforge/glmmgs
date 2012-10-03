#ifndef UTILITIES_ImmutableMatrix_H
#define UTILITIES_ImmutableMatrix_H

#include "Exceptions/Assertions.h"
#include "New.h"
#include "ReferenceCounter.h"
#include "External.h"

namespace Utilities
{
	// ImmutableMatrix
	template <class TYPE>
	class ImmutableMatrix
	{
	protected:
		// Fields
		TYPE * ptr;
		ReferenceCounter counter;
		int nrows;
		int ncols;

	public:
		// Construction
		ImmutableMatrix();	// Default
		ImmutableMatrix(int nrows, int ncols);	// Size
		ImmutableMatrix(External<TYPE> ptr, int nrows, int ncols); // From an external pointer
		~ImmutableMatrix();

		// Properties
		int NumberOfRows() const;
		int NumberOfColumns() const;

		// Assignment
		const ImmutableMatrix<TYPE> & operator =(const ImmutableMatrix<TYPE> & src);

		// Element access
		const TYPE & operator ()(int i, int j) const;
	};


	// Definition

	// Construction
	template <class TYPE> inline
	ImmutableMatrix<TYPE>::ImmutableMatrix()
		: ptr(NULL), nrows(0), ncols(0)
	{
	}

	template <class TYPE> inline
	ImmutableMatrix<TYPE>::ImmutableMatrix(int nrows, int ncols)
		: ptr(NewAllocator<TYPE>::New(nrows * ncols)), counter(ptr), nrows(nrows), ncols(ncols)
	{
	}

	template <class TYPE> inline
	ImmutableMatrix<TYPE>::ImmutableMatrix(External<TYPE> ptr, int nrows, int ncols)
		: ptr(ptr), counter(NULL), nrows(nrows), ncols(ncols)
	{
	}

	template <class TYPE> inline
	ImmutableMatrix<TYPE>::~ImmutableMatrix()
	{
		if (this->counter.Detach() == 0)
			NewAllocator<TYPE>::Delete(this->ptr);
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

	// Assignment
	template <class TYPE>
	const ImmutableMatrix<TYPE> & ImmutableMatrix<TYPE>::operator =(const ImmutableMatrix<TYPE> & src)
	{
		if (this->ptr != src.ptr)
		{
			// Copy reference counter
			if (this->counter.Detach() == 0)
				NewAllocator<TYPE>::Delete(this->ptr);
			this->counter.Attach(src.counter);

			// Copy members
			this->ptr = src.ptr;
			this->nrows = src.nrows;
			this->ncols = src.ncols;
		}
		return *this;
	}

	// Element access
	template <class TYPE> inline
	const TYPE & ImmutableMatrix<TYPE>::operator ()(int i, int j) const
	{
		_ASSERT(i >= 0 && i < this->nrows && j >= 0 && j < this->ncols);
		return this->ptr[i * this->ncols + j];
	}

	// Matrix
	template <class TYPE>
	class Matrix : public ImmutableMatrix<TYPE>
	{
	public:
		// Construction
		Matrix();	// Default
		Matrix(int nrows, int ncols);	// Size
		Matrix(External<TYPE> ptr, int nrows, int ncols); // From an external pointer

		// Element access
		TYPE & operator ()(int i, int j);
		const TYPE & operator ()(int i, int j) const;
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
	Matrix<TYPE>::Matrix(External<TYPE> ptr, int nrows, int ncols)
		: ImmutableMatrix<TYPE>(ptr, nrows, ncols)
	{
	}

	// Element access
	template <class TYPE> inline
	TYPE & Matrix<TYPE>::operator ()(int i, int j)
	{
		_ASSERT(i >= 0 && i < this->nrows && j >= 0 && j < this->ncols);
		return this->ptr[i * this->ncols + j];
	}

	template <class TYPE> inline
	const TYPE & Matrix<TYPE>::operator ()(int i, int j) const
	{
		_ASSERT(i >= 0 && i < this->nrows && j >= 0 && j < this->ncols);
		return this->ptr[i * this->ncols + j];
	}
}

#endif
