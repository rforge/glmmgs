#ifndef UTILITIES_FUNCTIONS_H
#define UTILITIES_FUNCTIONS_H

#include "Exceptions/Assertions.h"
#include "Vector.h"
#include "Matrix.h"

namespace Utilities
{
	// Vector assignment to a constant
	template <class TYPE>
	void Set(Vector<TYPE> & dst, const TYPE & x)
	{
		const int n = dst.Size();
		for (int i = 0; i < n; ++i)
			dst(i) = x;
	}

	// Copy vectors
	template <class TYPE>
	void Copy(Vector<TYPE> & dst, const Vector<TYPE> & src)
	{
		_ASSERT_ARGUMENT(dst.Size() == src.Size());
		for (int i = 0; i < dst.Size(); ++i)
			dst(i) = src(i);
	}

	// Min
	template <class TYPE>
	TYPE Min(const Vector<TYPE> & v)
	{
		_VALIDATE_ARGUMENT(v.Size() > 0);
		const int n = v.Size();
		TYPE min = v(0);
		for (int i = 1; i < n; ++i)
			if (min > v(i))
				min = v(i);
		return min;
	}

	// Max
	template <class TYPE>
	TYPE Max(const Vector<TYPE> & v)
	{
		_VALIDATE_ARGUMENT(v.Size() > 0);
		const int n = v.Size();
		TYPE max = v(0);
		for (int i = 1; i < n; ++i)
			if (max < v(i))
				max = v(i);
		return max;
	}

	// Vectorial max - TODO: test this
	template <class TYPE>
	TYPE Max(const Vector<Vector<TYPE> > & v)
	{
		_VALIDATE_ARGUMENT(v.Size() > 0);
		const int n = v.Size();
		TYPE max = Max(v(0));
		for (int i = 1; i < n; ++i)
			max = Max(max, Max(v(i)));
		return max;
	}

	// IsSorted
	template <class TYPE>
	bool IsSorted(const Vector<TYPE> & v)
	{
		const int n = v.Size();
		for (int i = 1; i < n; ++i)
			if (!(v(i - 1) <= v(i)))
				return false;
		return true;
	}
}

#endif
