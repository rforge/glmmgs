#ifndef UTILITIES_VECTOR_H
#define UTILITIES_VECTOR_H

#include "Exceptions/Assertions.h"
#include "External.h"
#include "Array.h"

namespace Utilities
{
	// ImmutableVector
	template <class TYPE>
	class ImmutableVector
	{
		template <class OTHER> friend class Cast;

	protected:
		// Fields
		Array<TYPE> array;
		int size;

	public:
		// Construction
		ImmutableVector();
		explicit ImmutableVector(int size);
		ImmutableVector(External<TYPE> ext, int size);

		// Properties
		int Size() const;

		// Element access
		const typename Immutable<TYPE>::Type & operator ()(int i) const;
	};

	// Definition

	// Construction
	template <class TYPE> inline
	ImmutableVector<TYPE>::ImmutableVector()
		: size(0)
	{
	}

	template <class TYPE> inline
	ImmutableVector<TYPE>::ImmutableVector(int size)
		: array(size), size(size)
	{
	}

	template <class TYPE> inline
	ImmutableVector<TYPE>::ImmutableVector(External<TYPE> ext, int size)
		: array(ext, size), size(size)
	{
	}

	// Properties
	template <class TYPE> inline
	int ImmutableVector<TYPE>::Size() const
	{
		return this->size;
	}

	// Element access
	template <class TYPE> inline
	const typename Immutable<TYPE>::Type & ImmutableVector<TYPE>::operator ()(int i) const
	{
		_ASSERT(i >= 0 && i < this->size);
		return this->array[i];
	}

	// Vector - Mutable class
	template <class TYPE>
	class Vector : public ImmutableVector<TYPE>
	{
		template <class OTHER> friend class Cast;

	public:
		// Construction
		Vector();
		explicit Vector(int size);
		Vector(External<TYPE> ext, int size);

		// Element access
		TYPE & operator ()(int i);
		const typename Immutable<TYPE>::Type & operator ()(int i) const;
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
		return this->array[i];
	}

	template <class TYPE> inline
	const typename Immutable<TYPE>::Type & Vector<TYPE>::operator ()(int i) const
	{
		_ASSERT(i >= 0 && i < this->size);
		return this->array[i];
	}

	// Specialize Immutable class
	template <class TYPE>
	struct Immutable<Vector<TYPE> >
	{
		typedef ImmutableVector<TYPE> Type;
	};
}

#endif
