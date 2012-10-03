#ifndef UTILITIES_TRIANGULARMATRIX_H
#define UTILITIES_TRIANGULARMATRIX_H

#include "Exceptions/Assertions.h"
#include "New.h"
#include "ReferenceCounter.h"
#include "External.h"

namespace Utilities
{
	// ImmutableTriangularMatrix
	template <class TYPE>
	class ImmutableTriangularMatrix
	{
	protected:
		// Fields
		TYPE * ptr;
		ReferenceCounter counter;
		int nrows;

		// Helper function returning the number of elements associated with a row
		static int Count(int row);

	public:
		// Construction
		ImmutableTriangularMatrix();	// Default
		explicit ImmutableTriangularMatrix(int nrows);	// Size
		ImmutableTriangularMatrix(External<TYPE> ptr, int nrows); // From an external pointer
		~ImmutableTriangularMatrix();

		// Attributes
		int NumberOfRows() const;

		// Assignment
		const ImmutableTriangularMatrix<TYPE> & operator =(const ImmutableTriangularMatrix<TYPE> & src);

		// Element access
		const TYPE & operator ()(int i, int j) const;
	};

	// Construction
	template <class TYPE> inline
	ImmutableTriangularMatrix<TYPE>::ImmutableTriangularMatrix()
		: ptr(NULL), nrows(0)
	{
	}

	template <class TYPE> inline
	ImmutableTriangularMatrix<TYPE>::ImmutableTriangularMatrix(int nrows)
		: ptr(NewAllocator<TYPE>::New(ImmutableTriangularMatrix<TYPE>::Count(nrows))), counter(ptr), nrows(nrows)
	{
	}

	template <class TYPE> inline
	ImmutableTriangularMatrix<TYPE>::ImmutableTriangularMatrix(External<TYPE> ptr, int nrows)
		: ptr(ptr), counter(NULL), nrows(nrows)
	{
	}

	template <class TYPE> inline
	ImmutableTriangularMatrix<TYPE>::~ImmutableTriangularMatrix()
	{
		if (this->counter.Detach() == 0)
			NewAllocator<TYPE>::Delete(this->ptr);
	}

	// Properties
	template <class TYPE> inline
	int ImmutableTriangularMatrix<TYPE>::NumberOfRows() const
	{
		return this->nrows;
	}

	// Assignment
	template <class TYPE>
	const ImmutableTriangularMatrix<TYPE> & ImmutableTriangularMatrix<TYPE>::operator =(const ImmutableTriangularMatrix<TYPE> & src)
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
		}
		return *this;
	}

	// Element access
	template <class TYPE> inline
	const TYPE & ImmutableTriangularMatrix<TYPE>::operator ()(int i, int j) const
	{
		_ASSERT(i >= 0 && i < this->nrows && j >= 0 && j <= i);
		return this->ptr[ImmutableTriangularMatrix<TYPE>::Count(i) + j];
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
		TriangularMatrix();	// Default
		explicit TriangularMatrix(int nrows);	// Size
		TriangularMatrix(External<TYPE> ptr, int nrows); // From an external pointer

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
	TriangularMatrix<TYPE>::TriangularMatrix(External<TYPE> ptr, int nrows)
		: ImmutableTriangularMatrix<TYPE>(ptr, nrows)
	{
	}

	// Element access
	template <class TYPE> inline
	TYPE & TriangularMatrix<TYPE>::operator ()(int i, int j)
	{
		_ASSERT(i >= 0 && i < this->nrows && j >= 0 && j <= i);
		return this->ptr[ImmutableTriangularMatrix<TYPE>::Count(i) + j];
	}

	template <class TYPE> inline
	const TYPE & TriangularMatrix<TYPE>::operator ()(int i, int j) const
	{
		_ASSERT(i >= 0 && i < this->nrows && j >= 0 && j <= i);
		return this->ptr[ImmutableTriangularMatrix<TYPE>::Count(i) + j];
	}
}

#endif
