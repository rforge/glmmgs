#ifndef UTILITIES_CAST_H
#define UTILITIES_CAST_H

#include "Cast.h"
#include "Array.h"
#include "Vector.h"

namespace Utilities
{
	// Declaration
	template <class OTHER> class Cast;

	// Cast to raw pointer
	template <class TYPE>
	class Cast<TYPE *>
	{
	public:
		static TYPE * Source(Array<TYPE> & array) {return array.ptr;}
		static TYPE * Source(Vector<TYPE> & vector) {return vector.array.ptr;}
	};

	// Cast to const raw pointer
	template <class TYPE>
	class Cast<const TYPE *>
	{
	public:
		static const TYPE * Source(const ImmutableArray<TYPE> & array) {return array.ptr;}
		static const TYPE * Source(const ImmutableVector<TYPE> & vector) {return vector.array.ptr;}
	};

	// Cast to array
	template <class TYPE>
	class Cast<Array<TYPE> >
	{
	public:
		static Array<TYPE> & Source(Vector<TYPE> & vector) {return vector.array;}
	};

	// Cast to immutable array
	template <class TYPE>
	class Cast<ImmutableArray<TYPE> >
	{
	public:
		static const ImmutableArray<TYPE> & Source(const ImmutableVector<TYPE> & vector) {return vector.array;}
	};
}

#endif
