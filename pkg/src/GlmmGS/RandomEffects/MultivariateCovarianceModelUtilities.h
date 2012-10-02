#ifndef GLMMGS_RANDOMEFFECTS_MULTIVARIATECOVARIANCEMODELUTILITIES_H
#define GLMMGS_RANDOMEFFECTS_MULTIVARIATECOVARIANCEMODELUTILITIES_H

#include "../Standard.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		// From precision vector to variance matrix
		ImmutableTriangularMatrix<double> ThetaToVariance(const ImmutableVector<double> & theta, int nvars);

		// Product
		class FProduct
		{
		private:
			Vector<double> fproducts;

		public:
			FProduct(const ImmutableVector<Vector<double> > & x);
			double operator () (int i) const {return this->fproducts(i);}
		};

		// Trace
		template <class TYPE>
		class FTrace
		{
		private:
			Vector<double> ftraces;

		public:
			FTrace(const ImmutableTriangularMatrix<TYPE> & x);
			double operator () (int i) const {return this->ftraces(i);}
		};


		// SquareTrace
		template <class TYPE>
		class FSquareTrace
		{
		private:
			TriangularMatrix<double> fsquare_traces;

		public:
			FSquareTrace(const ImmutableTriangularMatrix<TYPE> & x);
			double operator () (int i, int j) const {return this->fsquare_traces(i, j);}
		};
	}
}

#endif
