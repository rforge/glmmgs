#ifndef UTILITIES_VECTORIALALGORITHMS_H
#define UTILITIES_VECTORIALALGORITHMS_H

#include "Exceptions/Assertions.h"
#include "Vector.h"
#include "Matrix.h"

namespace Utilities
{
	// Min
	template <class TYPE>
	TYPE Min(const ImmutableVector<TYPE> & v)
	{
		_ASSERT(v.Size() > 0);
		const int n = v.Size();
		TYPE min = v(0);
		for (int i = 1; i < n; ++i)
			if (min > v(i))
				min = v(i);
		return min;
	}

	// Max
	template <class TYPE>
	TYPE Max(const ImmutableVector<TYPE> & v)
	{
		_ASSERT(v.Size() > 0);
		const int n = v.Size();
		TYPE max = v(0);
		for (int i = 1; i < n; ++i)
			if (max < v(i))
				max = v(i);
		return max;
	}

	// Vectorial max - TODO: test this
	template <class TYPE>
	TYPE Max(const ImmutableVector<Vector<TYPE> > & v)
	{
		_ASSERT(v.Size() > 0);
		const int n = v.Size();
		TYPE max = Max(v(0));
		for (int i = 1; i < n; ++i)
			max = Max(max, Max(v(i)));
		return max;
	}

	// IsSorted
	template <class TYPE>
	bool IsSorted(const ImmutableVector<TYPE> & v)
	{
		const int n = v.Size();
		for (int i = 1; i < n; ++i)
			if (!(v(i - 1) <= v(i)))
				return false;
		return true;
	}
}

#endif
