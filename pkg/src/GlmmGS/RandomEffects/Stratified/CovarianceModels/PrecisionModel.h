#ifndef GLMMGS_RANDOMEFFECTS_STRATIFIED_COVARIANCE_MODELS_PRECISIONMODEL_H
#define GLMMGS_RANDOMEFFECTS_STRATIFIED_COVARIANCE_MODELS_PRECISIONMODEL_H

#include "../../../Standard.h"
#include "../../Working/Stratified/CovarianceModels/ICovarianceModel.h"
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
					Matrix<const double> precision;

					// Implementation
					Pointer<Working::Stratified::CovarianceModels::ICovarianceModel> CreateWorking() const;

				public:
					// Construction
					PrecisionModel(int nvars, Matrix<const double> precision);
					~PrecisionModel();
				};
			}
		}
	}
}

#endif
