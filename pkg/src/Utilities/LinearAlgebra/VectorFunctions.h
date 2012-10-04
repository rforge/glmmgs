#ifndef UTILITIES_LINEARALGEBRA_VECTORFUNCTIONS_H
#define UTILITIES_LINEARALGEBRA_VECTORFUNCTIONS_H

#include "../Utilities.h"

namespace Utilities 
{
	namespace LinearAlgebra
	{
		// Operators
		template <class TYPE> Vector<TYPE> & operator *=(Vector<TYPE> & v, double a);
		template <class TYPE> Vector<TYPE> & operator +=(Vector<TYPE> & v, double a);
		template <class TYPE> Vector<TYPE> & operator +=(Vector<TYPE> & v, const ImmutableVector<TYPE> & w);
		template <class TYPE> Vector<TYPE> & operator -=(Vector<TYPE> & v, double a);
		template <class TYPE> Vector<TYPE> & operator -=(Vector<TYPE> & v, const ImmutableVector<TYPE> & w);
		Vector<double> operator *(double a, const ImmutableVector<double> & v);
		Vector<double> operator +(const ImmutableVector<double> & v, const ImmutableVector<double> & w);
		Vector<double> operator -(const ImmutableVector<double> & v, const ImmutableVector<double> & w);

		// Functions
		double Sum(const ImmutableVector<double> & v);
		double ScalarProduct(const ImmutableVector<double> & v, const ImmutableVector<double> & w);
		double Square(const ImmutableVector<double> & v);
		double MaxAbs(const ImmutableVector<double> & v);
		double MinAbs(const ImmutableVector<double> & v);

		// Vectorial functions
		double ScalarProduct(const ImmutableVector<Vector<double> > & v, const ImmutableVector<Vector<double> > & w);
		double MaxAbs(const ImmutableVector<Vector<double> > & v);
		double MinAbs(const ImmutableVector<Vector<double> > & v);


		// Definition

		// Operators
		template <class TYPE>
		Vector<TYPE> & operator *=(Vector<TYPE> & v, double a)
		{
			const int n = v.Size();
			for (int i = 0; i < n; ++i)
				v(i) *= a;
			return v;
		}

		template <class TYPE>
		Vector<TYPE> & operator +=(Vector<TYPE> & v, double a)
		{
			const int n = v.Size();
			for (int i = 0; i < n; ++i)
				v(i) += a;
			return v;
		}

		template <class TYPE>
		Vector<TYPE> & operator +=(Vector<TYPE> & v, const ImmutableVector<TYPE> & w)
		{
			_ASSERT(v.Size() == w.Size());
			const int n = v.Size();
			for (int i = 0; i < n; ++i)
				v(i) += w(i);
			return v;
		}

		template <class TYPE>
		Vector<TYPE> & operator -=(Vector<TYPE> & v, double a)
		{
			const int n = v.Size();
			for (int i = 0; i < n; ++i)
				v(i) -= a;
			return v;
		}

		template <class TYPE>
		Vector<TYPE> & operator -=(Vector<TYPE> & v, const ImmutableVector<TYPE> & w)
		{
			_ASSERT(v.Size() == w.Size());
			const int n = v.Size();
			for (int i = 0; i < n; ++i)
				v(i) -= w(i);
			return v;
		}
	}
}

#endif
