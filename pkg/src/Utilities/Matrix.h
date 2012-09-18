#ifndef UTILITIES_MATRIX_H
#define UTILITIES_MATRIX_H

#include "Exceptions/Assertions.h"
#include "New.h"
#include "ReferenceCounter.h"
#include "External.h"

namespace Utilities
{
	// Matrix
	template <class TYPE>
	class Matrix
	{
		template <class OTHER> friend class Cast;

	private:
		// Fields
		TYPE * ptr;
		ReferenceCounter counter;
		int nrows;
		int ncols;

	public:
		// Construction
		Matrix();	// Default
		Matrix(int nrows, int ncols);	// Size
		Matrix(TYPE * ptr, const ReferenceCounter & counter, int nrows, int ncols); // From another ref-counted pointer
		Matrix(External<TYPE> ptr, int nrows, int ncols); // From an external pointer
		~Matrix();

		// Properties
		int NumberOfRows() const;
		int NumberOfColumns() const;

		// Assignment
		const Matrix<TYPE> & operator =(const Matrix<TYPE> & src);

		// Element access
		TYPE & operator ()(int i, int j);
		const TYPE & operator ()(int i, int j) const;
	};


	// Definition

	// Construction
	template <class TYPE> inline
	Matrix<TYPE>::Matrix()
		: ptr(NULL), nrows(0), ncols(0)
	{
	}

	template <class TYPE> inline
	Matrix<TYPE>::Matrix(int nrows, int ncols)
		: ptr(NewAllocator<TYPE>::New(nrows * ncols)), counter(ptr), nrows(nrows), ncols(ncols)
	{
	}

	template <class TYPE> inline
	Matrix<TYPE>::Matrix(TYPE * ptr, const ReferenceCounter & counter, int nrows, int ncols)
		: ptr(ptr), counter(counter), nrows(nrows), ncols(ncols)
	{
	}

	template <class TYPE> inline
	Matrix<TYPE>::Matrix(External<TYPE> ptr, int nrows, int ncols)
		: ptr(ptr), counter(NULL), nrows(nrows), ncols(ncols)
	{
	}

	template <class TYPE> inline
	Matrix<TYPE>::~Matrix()
	{
		if (this->counter.Decrement() == 0)
			NewAllocator<TYPE>::Delete(this->ptr);
	}

	// Properties
	template <class TYPE> inline
	int Matrix<TYPE>::NumberOfRows() const
	{
		return this->nrows;
	}

	template <class TYPE> inline
	int Matrix<TYPE>::NumberOfColumns() const
	{
		return this->ncols;
	}

	// Assignment
	template <class TYPE>
	const Matrix<TYPE> & Matrix<TYPE>::operator =(const Matrix<TYPE> & src)
	{
		if (this->ptr != src.ptr)
		{
			// Copy reference counter
			if (this->counter.Decrement() == 0)
				NewAllocator<TYPE>::Delete(this->ptr);
			this->counter.Increment(src.counter);

			// Copy members
			this->ptr = src.ptr;
			this->nrows = src.nrows;
			this->ncols = src.ncols;
		}
		return *this;
	}

	// Element access
	template <class TYPE> inline
	TYPE & Matrix<TYPE>::operator ()(int i, int j)
	{
		_ASSERT_ARGUMENT(i >= 0 && i < this->nrows && j >= 0 && j < this->ncols)
		return this->ptr[i * this->ncols + j];
	}

	template <class TYPE> inline
	const TYPE & Matrix<TYPE>::operator ()(int i, int j) const
	{
		_ASSERT_ARGUMENT(i >= 0 && i < this->nrows && j >= 0 && j < this->ncols)
		return this->ptr[i * this->ncols + j];
	}
}

#endif
