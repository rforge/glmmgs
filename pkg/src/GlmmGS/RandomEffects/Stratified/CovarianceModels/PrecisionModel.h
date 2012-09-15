#ifndef GLMMGS_RANDOMEFFECTS_STRATIFIED_COVARIANCEMODELS_PRECISIONMODEL_H
#define GLMMGS_RANDOMEFFECTS_STRATIFIED_COVARIANCEMODELS_PRECISIONMODEL_H

#include "../../../Standard.h"
#include "ICovarianceModel.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Stratified
		{
			namespace CovarianceModels
			{
				// PrecisionModel
				class PrecisionModel : public ICovarianceModel
				{
				private:
					// Fields
					int nvars;
					Matrix<const double> R;
					CholeskyDecomposition beta_precision_chol;

					// Implementation
					void Decompose(const TriangularMatrix<Vector<double> > & precision);
					int Update(const Vector<Vector<double> > & beta, const Controls & controls);
					Vector<Vector<double> > UpdateCoefficients(const Vector<Vector<double> > & jacobian, const Vector<Vector<double> > & beta) const;

				public:
					// Construction
					PrecisionModel(int nvars, Matrix<const double> R);
					~PrecisionModel();
				};
			}
		}
	}
}

#endif
