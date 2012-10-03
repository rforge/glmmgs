#ifndef GLMMGS_RANDOMEFFECTS_COVARIANCEMODELUTILITIES_H
#define GLMMGS_RANDOMEFFECTS_COVARIANCEMODELUTILITIES_H

#include "../Standard.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		void ValidateTheta(const ImmutableVector<double> & theta, const ImmutableVector<double> & init_theta);

		// Weighted squared norm of a vector
		double Square(const ImmutableMatrix<double> & m, const ImmutableVector<double> & x);
		double Square(const LDL::SparseMatrix<double> & R, const ImmutableVector<double> & x);

		// Trace of diagonal blocks
		double BlockTrace(int row, int nlevels, const ImmutableMatrix<double> & a);

		// Trace of diagonal blocks product
		double BlockSquareTrace(int row, int col, int nlevels, const ImmutableMatrix<double> & a);

		// Matrix product
		double MatrixProduct(int k, const ImmutableMatrix<double> & m, const ImmutableVector<double> & x);

		// Transposed-matrix product
		double TMatrixProduct(int k, const LDL::SparseMatrix<double> & R, const ImmutableVector<double> & x);
	}
}

#endif
