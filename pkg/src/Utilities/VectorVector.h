#ifndef UTILITIES_VECTORVECTOR_H
#define UTILITIES_VECTORVECTOR_H

#include "Vector.h"

namespace Utilities
{
	// Build stratified vector
	template <class TYPE>
	Vector<Vector<TYPE> > VectorVector(int n, int m)
	{
		Vector<Vector<TYPE> > v(n);
		for (int i = 0; i < n; ++i)
			v(i) = Vector<TYPE>(m);
		return v;
	}

	// Size of a stratified vector
	template <class TYPE>
	int TotalSize(const ImmutableVector<Vector<TYPE> > & v)
	{
		const int n = v.Size();
		int size = 0;
		for (int i = 0; i < n; ++i)
			size += v(i).Size();
		return size;
	}
}

#endif
