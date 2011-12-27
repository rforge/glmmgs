#ifndef GLMMGS_RANDOMEFFECTS_WORKING_COVARIANCEMODEL_PRECISIONMODEL_H
#define GLMMGS_RANDOMEFFECTS_WORKING_COVARIANCEMODEL_PRECISIONMODEL_H

#include "../../../../Standard.h"
#include "../../../../Estimate.h"
#include "ICovarianceModel.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Working
		{
			namespace Global
			{
				namespace CovarianceModels
				{
					// PrecisionModel
					class PrecisionModel : public ICovarianceModel
					{
					private:
						// Fields
						Matrix<const double> R;
						CholeskyDecomposition beta_precision_chol;

						// Implementation
						Vector<double> CoefficientsVariance() const;
						void Decompose(const TriangularMatrix<double> & precision);
						int Update(const Vector<double> & beta, const Controls & controls);
						Vector<double> CoefficientsUpdate(const Vector<double> & jacobian, const Vector<double> & beta) const;

					public:
						// Construction
						PrecisionModel(Matrix<const double> R);
						~PrecisionModel();
					};
				}
			}
		}
	}
}

#endif
