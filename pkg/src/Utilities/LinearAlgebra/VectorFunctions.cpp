#include "../Utilities.h"
#include "../Math/Math.h"
#include "VectorFunctions.h"

namespace Utilities 
{
	namespace LinearAlgebra
	{
		// Operators
		Vector<double> operator *(double a, const ImmutableVector<double> & v)
		{
			const int n = v.Size();
			Vector<double> y(n);
			for (int i = 0; i < n; ++i)
				y(i) = a * v(i);
			return y;
		}

		Vector<double> operator +(const ImmutableVector<double> & v, const ImmutableVector<double> & w)
		{
			_ASSERT(v.Size() == w.Size());
			const int n = v.Size();
			Vector<double> y(n);
			for (int i = 0; i < n; ++i)
				y(i) = v(i) + w(i);
			return y;
		}

		Vector<double> operator -(const ImmutableVector<double> & v, const ImmutableVector<double> & w)
		{
			_ASSERT(v.Size() == w.Size());
			const int n = v.Size();
			Vector<double> y(n);
			for (int i = 0; i < n; ++i)
				y(i) = v(i) - w(i);
			return y;
		}

		// Functions
		double Sum(const ImmutableVector<double> & v)
		{
			const int n = v.Size();
			double sum = 0.0;
			for (int i = 0; i < n; ++i)
				sum += v(i);
			return sum;
		}

		double ScalarProduct(const ImmutableVector<double> & v, const ImmutableVector<double> & w)
		{
			_ASSERT(v.Size() == w.Size());
			const int n = v.Size();
			double sum = 0.0;
			for (int i = 0; i < n; ++i)
				sum += v(i) * w(i);
			return sum;
		}

		double Square(const ImmutableVector<double> & v)
		{
			const int n = v.Size();
			double sum = 0.0;
			for (int i = 0; i < n; ++i)
				sum += Math::Square(v(i));
			return sum;
		}
		
		// Max-Min Abs implementation
		double MaxAbsImpl(double x)
		{
			return fabs(x);
		}

		double MinAbsImpl(double x)
		{
			return fabs(x);
		}

		template <class TYPE>
		double MaxAbsImpl(const ImmutableVector<TYPE> & v)
		{
			const int n = v.Size();
			_ASSERT(n > 0);
			double max = MaxAbsImpl(v(0));
			for (int i = 1; i < n; ++i)
			{
				const double a = MaxAbsImpl(v(i));
				if (a > max)
					max = a;
			}
			return max;
		}

		template <class TYPE>
		double MinAbsImpl(const ImmutableVector<TYPE> & v)
		{
			const int n = v.Size();
			_ASSERT(n > 0);
			double min = MinAbsImpl(v(0));
			for (int i = 1; i < n; ++i)
			{
				const double a = MinAbsImpl(v(i));
				if (a < min)
					min = a;
			}
			return min;
		}

		// Min-Max Abs
		double MaxAbs(const ImmutableVector<double> & v)
		{
			return MaxAbsImpl(v);
		}

		double MinAbs(const ImmutableVector<double> & v)
		{
			return MinAbsImpl(v);
		}

		// Vectorial functions
		double ScalarProduct(const Vector<Vector<double> > & x, const Vector<Vector<double> > & y)
		{
			_ASSERT(x.Size() == y.Size());
			const int n = x.Size();
			double sum = 0;
			for (int i = 0; i < n; ++i)
				sum += ScalarProduct(x(i), y(i));
			return sum;
		}

		// Min-Max Abs
		double MaxAbs(const Vector<Vector<double> > & v)
		{
			return MaxAbsImpl(v);
		}

		double MinAbs(const Vector<Vector<double> > & v)
		{
			return MinAbsImpl(v);
		}
	}
}
