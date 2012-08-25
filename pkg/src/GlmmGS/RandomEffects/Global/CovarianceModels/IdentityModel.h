#ifndef GLMMGS_RANDOMEFFECTS_COVARIANCEMODEL_IDENTITYMODEL_H
#define GLMMGS_RANDOMEFFECTS_COVARIANCEMODEL_IDENTITYMODEL_H

#include "../../../Standard.h"
#include "../../../Estimate.h"
#include "ICovarianceModel.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Global
		{
			namespace CovarianceModels
			{
				// IdentityModel
				class IdentityModel : public ICovarianceModel
				{
				private:
					// Fields
					int size;
					CholeskyDecomposition beta_precision_chol;

					// Implementation
					Vector<double> CoefficientsVariance() const;
					void Decompose(const TriangularMatrix<double> & precision);
					int Update(const Vector<double> & beta, const Controls & controls);
					Vector<double> UpdateCoefficients(const Vector<double> & jacobian, const Vector<double> & beta) const;

				public:
					// Construction
					IdentityModel(int nvars);
					~IdentityModel();
				};
			}
		}
	}
}

#endif
