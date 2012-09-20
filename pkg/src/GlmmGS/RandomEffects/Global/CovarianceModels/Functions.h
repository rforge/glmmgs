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
				double Square(Matrix<const double> m, const ImmutableVector<double> & x);

				// Trace
				double Trace(const Matrix<double> & a);
				// Trace of square product
				double SquareTrace(const Matrix<double> & a);

				// Matrix-vector product
				double MatrixProduct(int k, Matrix<const double> m, const ImmutableVector<double> & x);
			}
		}
	}
}

#endif
