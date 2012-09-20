#ifndef UTILITIES_ARRAY_H
#define UTILITIES_ARRAY_H

#include "Exceptions/Assertions.h"
#include "New.h"
#include "ReferenceCounter.h"
#include "External.h"

namespace Utilities
{
	// ImmutableArray
	template <class TYPE>
	class ImmutableArray
	{
		template <class OTHER> friend class Cast;

	protected:
		// Fields
		TYPE * ptr;
		ReferenceCounter counter;

#ifdef _DEBUG
		int size;
#endif

	public:
		// Construction
		ImmutableArray();	// Default
		explicit ImmutableArray(int size);	// Size
		ImmutableArray(TYPE * ptr, ReferenceCounter counter, int size); // From a referenced counted pointer
		~ImmutableArray();

		// Assignment
		const ImmutableArray<TYPE> & operator =(const ImmutableArray<TYPE> & src);

		// Cast
		operator bool () const {return this->ptr != NULL;}

		// Element access
		const TYPE & operator [](int i) const;
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
		: ptr(NewAllocator<TYPE>::New(size)), counter(ptr), size(size)
#else
		: ptr(NewAllocator<TYPE>::New(size)), counter(ptr)
#endif
	{
	}

	template <class TYPE> inline
	ImmutableArray<TYPE>::ImmutableArray(TYPE * ptr, ReferenceCounter counter, int size)
#ifdef _DEBUG
		: ptr(ptr), counter(counter), size(size)
#else
		: ptr(ptr), counter(NULL)
#endif
	{
	}

	template <class TYPE> inline
	ImmutableArray<TYPE>::~ImmutableArray()
	{
		if (this->counter.Detach() == 0)
			NewAllocator<TYPE>::Delete(this->ptr);
	}

	// Assignment
	template <class TYPE>
	const ImmutableArray<TYPE> & ImmutableArray<TYPE>::operator =(const ImmutableArray<TYPE> & src)
	{
		if (this->ptr != src.ptr)
		{
			// Copy references
			if (this->counter.Detach() == 0)
				NewAllocator<TYPE>::Delete(this->ptr);
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
	const TYPE & ImmutableArray<TYPE>::operator [](int i) const
	{
		_ASSERT_ARGUMENT(i >= 0 && i < this->size);
		return this->ptr[i];
	}

	// Array - Mutable class
	template <class TYPE>
	class Array : public ImmutableArray<TYPE>
	{
		template <class OTHER> friend class Cast;

	public:
		Array();	// Default
		explicit Array(int size);	// Size
		Array(TYPE * ptr, ReferenceCounter counter, int size); // From a referenced counted pointer


		// Element access
		TYPE & operator [](int i);
		const TYPE & operator [](int i) const;
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
	Array<TYPE>::Array(TYPE * ptr, ReferenceCounter counter, int size)
		: ImmutableArray<TYPE>(ptr, counter, size)
	{
	}

	// Element access
	template <class TYPE> inline
	TYPE & Array<TYPE>::operator [](int i)
	{
		_ASSERT_ARGUMENT(i >= 0 && i < this->size);
		return this->ptr[i];
	}

	template <class TYPE> inline
	const TYPE & Array<TYPE>::operator [](int i) const
	{
		_ASSERT_ARGUMENT(i >= 0 && i < this->size);
		return this->ptr[i];
	}
}

#endif
