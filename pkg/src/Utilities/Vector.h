#ifndef UTILITIES_VECTOR_H
#define UTILITIES_VECTOR_H

#include "Exceptions/Assertions.h"
#include "New.h"
#include "ReferenceCounter.h"
#include "External.h"

namespace Utilities
{
	// ImmutableVector
	template <class TYPE>
	class ImmutableVector
	{
		template <class OTHER> friend class Cast;

	protected:
		// Fields
		TYPE * ptr;
		ReferenceCounter counter;
		int size;

	public:
		// Construction
		ImmutableVector();	// Default
		explicit ImmutableVector(int size);	// Size
		ImmutableVector(External<TYPE> ext, int size); // From an external pointer
		~ImmutableVector();

		// Properties
		int Size() const;

		// Assignment
		const ImmutableVector<TYPE> & operator =(const ImmutableVector<TYPE> & src);

		// Element access
		const TYPE & operator ()(int i) const;
	};

	// Definition

	// Construction
	template <class TYPE> inline
	ImmutableVector<TYPE>::ImmutableVector()
		: ptr(NULL), size(0)
	{
	}

	template <class TYPE> inline
	ImmutableVector<TYPE>::ImmutableVector(int size)
		: ptr(NewAllocator<TYPE>::New(size)), counter(ptr), size(size)
	{
	}

	template <class TYPE> inline
	ImmutableVector<TYPE>::ImmutableVector(External<TYPE> ext, int size)
		: ptr(ext), counter(NULL), size(size) // const_cast is safe since the object is immutable
	{
	}

	template <class TYPE> inline
	ImmutableVector<TYPE>::~ImmutableVector()
	{
		if (this->counter.Detach() == 0)
			NewAllocator<TYPE>::Delete(this->ptr);
	}

	// Properties
	template <class TYPE> inline
	int ImmutableVector<TYPE>::Size() const
	{
		return this->size;
	}

	// Assignment
	template <class TYPE>
	const ImmutableVector<TYPE> & ImmutableVector<TYPE>::operator =(const ImmutableVector<TYPE> & src)
	{
		if (this->ptr != src.ptr)
		{
			// Copy references
			if (this->counter.Detach() == 0)
				NewAllocator<TYPE>::Delete(this->ptr);
			this->counter.Attach(src.counter);

			// Copy members
			this->ptr = src.ptr;
			this->size = src.size;
		}
		return *this;
	}

	// Element access
	template <class TYPE> inline
	const TYPE & ImmutableVector<TYPE>::operator ()(int i) const
	{
		_ASSERT(i >= 0 && i < this->size);
		return this->ptr[i];
	}

	// Vector - Mutable class
	template <class TYPE>
	class Vector : public ImmutableVector<TYPE>
	{
		template <class OTHER> friend class Cast;

	public:
		// Construction
		Vector();	// Default
		explicit Vector(int size);	// Size
		Vector(External<TYPE> ext, int size); // From an external pointer

		// Element access
		TYPE & operator ()(int i);
		const TYPE & operator ()(int i) const;
	};

	// Definition

	// Construction
	template <class TYPE> inline
	Vector<TYPE>::Vector()
	{
	}

	template <class TYPE> inline
	Vector<TYPE>::Vector(int size)
		: ImmutableVector<TYPE>(size)
	{
	}

	template <class TYPE> inline
	Vector<TYPE>::Vector(External<TYPE> ext, int size)
		: ImmutableVector<TYPE>(ext, size)
	{
	}

	// Element access
	template <class TYPE> inline
	TYPE & Vector<TYPE>::operator ()(int i)
	{
		_ASSERT(i >= 0 && i < this->size);
		return this->ptr[i];
	}

	template <class TYPE> inline
	const TYPE & Vector<TYPE>::operator ()(int i) const
	{
		_ASSERT(i >= 0 && i < this->size);
		return this->ptr[i];
	}
}

#endif
