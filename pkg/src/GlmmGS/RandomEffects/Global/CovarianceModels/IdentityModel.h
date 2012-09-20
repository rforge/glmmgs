#ifndef GLMMGS_RANDOMEFFECTS_COVARIANCEMODEL_IDENTITYMODEL_H
#define GLMMGS_RANDOMEFFECTS_COVARIANCEMODEL_IDENTITYMODEL_H

#include "../../../Standard.h"
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
					const int size;

					// Implementation
					void Decompose(const TriangularMatrix<double> & precision);
					int Update(const ImmutableVector<double> & beta, const Controls & controls);
					Vector<double> UpdateCoefficients(const ImmutableVector<double> & jacobian, const ImmutableVector<double> & beta) const;

				public:
					// Construction
					IdentityModel(int nvars, Matrix<const double> S);
					~IdentityModel();
				};
			}
		}
	}
}

#endif
