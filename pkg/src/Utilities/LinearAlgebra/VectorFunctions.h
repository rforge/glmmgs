#ifndef UTILITIES_LINEARALGEBRA_VECTORFUNCTIONS_H
#define UTILITIES_LINEARALGEBRA_VECTORFUNCTIONS_H

#include "../Utilities.h"

namespace Utilities 
{
	namespace LinearAlgebra
	{
		// Operators
		Vector<double> & operator *=(Vector<double> & v, double a);
		template <class TYPE> Vector<TYPE> & operator +=(Vector<TYPE> & v, const Vector<TYPE> & w);
		Vector<double> & operator -=(Vector<double> & v, const Vector<double> & w);
		Vector<double> operator *(double a, const Vector<double> & v);
		Vector<double> operator +(const Vector<double> & v, const Vector<double> & w);
		Vector<double> operator -(const Vector<double> & v, const Vector<double> & w);

		// Functions
		double Sum(const Vector<double> & v);
		double ScalarProduct(const Vector<double> & v, const Vector<double> & w);
		double Square(const Vector<double> & v);
		double MaxAbs(const Vector<double> & v);
		double MinAbs(const Vector<double> & v);

		// Vectorial functions
		double ScalarProduct(const Vector<Vector<double> > & v, const Vector<Vector<double> > & w);
		double MaxAbs(const Vector<Vector<double> > & v);
		double MinAbs(const Vector<Vector<double> > & v);


	// Definition
		// Operators
		template <class TYPE>
		Vector<TYPE> & operator +=(Vector<TYPE> & v, const Vector<TYPE> & w)
		{
			_ASSERT_ARGUMENT(v.Size() == w.Size());
			const int n = v.Size();
			for (int i = 0; i < n; ++i)
				v(i) += w(i);
			return v;
		}

		// Vectorial functions
	}
}

#endif
