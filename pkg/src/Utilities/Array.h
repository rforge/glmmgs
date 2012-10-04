#ifndef UTILITIES_ARRAY_H
#define UTILITIES_ARRAY_H

#include "Exceptions/Assertions.h"
#include "Memory.h"
#include "ReferenceCounter.h"
#include "External.h"
#include "Immutable.h"

namespace Utilities
{
	// ImmutableArray
	template <class TYPE>
	class ImmutableArray
	{
		// Cast class
		template <class OTHER> friend class Cast;

	private:
		// Disable assignment
		const ImmutableArray<TYPE> & operator =(const ImmutableArray<TYPE> & src);

	protected:
		// Fields
		TYPE * ptr;
		ReferenceCounter counter;
#ifdef _DEBUG
		int size;
#endif

	public:
		// Construction
		ImmutableArray();
		explicit ImmutableArray(int size);
		ImmutableArray(External<TYPE> ext, int size);
		~ImmutableArray();

		// Test for null pointer
		operator bool () const;

		// Element access
		const typename Immutable<TYPE>::Type & operator [](int i) const;
	};

	// Definition

	// Construction
	template <class TYPE> inline
	ImmutableArray<TYPE>::ImmutableArray()
#ifdef _DEBUG
		: ptr(NULL), size(0)
#else
		: ptr(NULL)
#endif
	{
	}

	template <class TYPE> inline
	ImmutableArray<TYPE>::ImmutableArray(int size)
#ifdef _DEBUG
		: ptr(Allocator<TYPE>::New(size)), counter(ptr), size(size)
#else
		: ptr(Allocator<TYPE>::New(size)), counter(ptr)
#endif
	{
	}

	template <class TYPE> inline
	ImmutableArray<TYPE>::ImmutableArray(External<TYPE> ptr, int size)
#ifdef _DEBUG
		: ptr(ptr), counter(NULL), size(size)
#else
		: ptr(ptr), counter(NULL)
#endif
	{
	}

	template <class TYPE> inline
	ImmutableArray<TYPE>::~ImmutableArray()
	{
		if (this->counter.Detach() == 0)
			Allocator<TYPE>::Delete(this->ptr);
	}

	// Check for NULL pointer
	template <class TYPE> inline
	ImmutableArray<TYPE>::operator bool () const
	{
		return this->ptr != NULL;
	}

	// Element access
	template <class TYPE> inline
	const typename Immutable<TYPE>::Type & ImmutableArray<TYPE>::operator [](int i) const
	{
		_ASSERT(i >= 0 && i < this->size);
		return this->ptr[i];
	}

	// Array - Mutable class
	template <class TYPE>
	class Array : public ImmutableArray<TYPE>
	{
		template <class OTHER> friend class Cast;

	public:
		Array();
		explicit Array(int size);
		Array(External<TYPE> ext, int size);

		// Assignment
		const Array<TYPE> & operator =(const Array<TYPE> & src);

		// Element access
		TYPE & operator [](int i);
		const typename Immutable<TYPE>::Type & operator [](int i) const;
	};

	// Definition

	// Construction
	template <class TYPE> inline
	Array<TYPE>::Array()
	{
	}

	template <class TYPE> inline
	Array<TYPE>::Array(int size)
		: ImmutableArray<TYPE>(size)
	{
	}

	template <class TYPE> inline
	Array<TYPE>::Array(External<TYPE> ext, int size)
		: ImmutableArray<TYPE>(ext, size)
	{
	}

	// Assignment
	template <class TYPE>
	const Array<TYPE> & Array<TYPE>::operator =(const Array<TYPE> & src)
	{
		if (this->ptr != src.ptr)
		{
			// Copy references
			if (this->counter.Detach() == 0)
				Allocator<TYPE>::Delete(this->ptr);
			this->counter.Attach(src.counter);

			// Copy members
			this->ptr = src.ptr;
#ifdef _DEBUG
			this->size = src.size;
#endif
		}
		return *this;
	}

	// Element access
	template <class TYPE> inline
	TYPE & Array<TYPE>::operator [](int i)
	{
		_ASSERT(i >= 0 && i < this->size);
		return this->ptr[i];
	}

	template <class TYPE> inline
	const typename Immutable<TYPE>::Type & Array<TYPE>::operator [](int i) const
	{
		_ASSERT(i >= 0 && i < this->size);
		return this->ptr[i];
	}

	// Specialize Immutable class
	template <class TYPE>
	struct Immutable<Array<TYPE> >
	{
		typedef ImmutableArray<TYPE> Type;
	};
}

#endif
