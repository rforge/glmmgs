#ifndef GLMMGS_RANDOMEFFECTS_FUNCTIONS_PRECISIONMODEL_H
#define GLMMGS_RANDOMEFFECTS_FUNCTIONS_PRECISIONMODEL_H

#include "../../../Standard.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Global
		{
			namespace CovarianceModels
			{
				// Weighted square norm of vector
				double Square(const ImmutableMatrix<double> & m, const ImmutableVector<double> & x);

				// Trace
				double Trace(const ImmutableMatrix<double> & a);
				// Trace of square product
				double SquareTrace(const ImmutableMatrix<double> & a);

				// Matrix-vector product
				double MatrixProduct(int k, const ImmutableMatrix<double> & m, const ImmutableVector<double> & x);
			}
		}
	}
}

#endif
