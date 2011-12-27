#ifndef UTILITIES_TRIANGULARMATRIX_H
#define UTILITIES_TRIANGULARMATRIX_H

#include "Exceptions/Assertions.h"
#include "New.h"
#include "ReferenceCounter.h"
#include "External.h"

namespace Utilities
{
	// Matrix
	template <class TYPE>
	class TriangularMatrix
	{
		template <class OTHER> friend class Cast;

	private:
		// Fields
		TYPE * ptr;
		ReferenceCounter counter;
		int nrows;

		// Implementation
		static int Count(int row);

	public:
		// Construction
		TriangularMatrix();	// Default
		explicit TriangularMatrix(int nrows);	// Size
		TriangularMatrix(TYPE * ptr, const ReferenceCounter & counter, int nrows); // From another ref-counted pointer
		TriangularMatrix(External<TYPE> ptr, int nrows); // From an external pointer
		~TriangularMatrix();

		// Attributes
		int NumberOfRows() const;

		// Element access
		TYPE & operator ()(int i, int j);
		const TYPE & operator ()(int i, int j) const;

		// Assignment
		const TriangularMatrix<TYPE> & operator =(const TriangularMatrix<TYPE> & src);
	};

	// Construction
	template <class TYPE> inline
	TriangularMatrix<TYPE>::TriangularMatrix()
		: ptr(NULL), nrows(0)
	{
	}

	template <class TYPE> inline
	TriangularMatrix<TYPE>::TriangularMatrix(int nrows)
		: ptr(NewAllocator<TYPE>::New(TriangularMatrix<TYPE>::Count(nrows))), counter(ptr), nrows(nrows)
	{
	}

	template <class TYPE> inline
	TriangularMatrix<TYPE>::TriangularMatrix(TYPE * ptr, const ReferenceCounter & counter, int nrows)
		: ptr(ptr), counter(counter), nrows(nrows)
	{
	}

	template <class TYPE> inline
	TriangularMatrix<TYPE>::TriangularMatrix(External<TYPE> ptr, int nrows)
		: ptr(ptr), counter(NULL), nrows(nrows)
	{
	}

	template <class TYPE> inline
	TriangularMatrix<TYPE>::~TriangularMatrix()
	{
		if (this->counter.Decrement() == 0)
			NewAllocator<TYPE>::Delete(this->ptr);
	}

	// Properties
	template <class TYPE> inline
	int TriangularMatrix<TYPE>::NumberOfRows() const
	{
		return this->nrows;
	}

	// Assignment
	template <class TYPE>
	const TriangularMatrix<TYPE> & TriangularMatrix<TYPE>::operator =(const TriangularMatrix<TYPE> & src)
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
		}
		return *this;
	}

	// Element access
	template <class TYPE> inline
	TYPE & TriangularMatrix<TYPE>::operator ()(int i, int j)
	{
		_ASSERT_ARGUMENT(i >= 0 && i < this->nrows && j >= 0 && j <= i);
		return this->ptr[TriangularMatrix::Count(i) + j];
	}

	template <class TYPE> inline
	const TYPE & TriangularMatrix<TYPE>::operator ()(int i, int j) const
	{
		_ASSERT_ARGUMENT(i >= 0 && i < this->nrows && j >= 0 && j <= i);
		return this->ptr[TriangularMatrix::Count(i) + j];
	}

	// Implementation
	template <class TYPE> inline
	int TriangularMatrix<TYPE>::Count(int row)
	{
		return (row * (row + 1)) >> 1;
	}
}

#endif
