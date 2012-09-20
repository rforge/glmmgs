#ifndef UTILITIES_ARRAYCAST_H
#define UTILITIES_ARRAYCAST_H

#include "Cast.h"
#include "Array.h"
#include "Vector.h"

namespace Utilities
{
	// Cast class
	template <class TYPE>
	class Cast<TYPE *>
	{
	private:
		TYPE * ptr;

	public:
		// Cast vector
		Cast(Vector<TYPE> & src)
			: ptr(src.ptr)
		{
		}

		operator TYPE *() {return ptr;}
	};

	// Cast class
	template <class TYPE>
	class Cast<const TYPE *>
	{
	private:
		const TYPE * ptr;

	public:
		// Cast vector
		Cast(const ImmutableVector<TYPE> & src)
			: ptr(src.ptr)
		{
		}

		operator const TYPE *() const {return ptr;}
	};

	// Cast class
	template <class TYPE>
	class Cast<Array<TYPE> > : public Array<TYPE>
	{
		typedef Array<TYPE> To;

	public:
		// Cast vector
		Cast(Vector<TYPE> & src)
			: To(src.ptr, src.counter, src.size)
		{
		}
	};

	// Const cast class
	template <class TYPE>
	class Cast<ImmutableArray<TYPE> > : public ImmutableArray<TYPE>
	{
		typedef const ImmutableArray<TYPE> To;

	public:
		// Cast vector
		Cast(const ImmutableVector<TYPE> & src)
			: To(src.ptr, src.counter, src.size)
		{
		}
	};
}

#endif
