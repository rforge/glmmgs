#ifndef GLMMGS_RANDOMEFFECTS_COVARIANCEMODEL_PRECISIONMODEL_H
#define GLMMGS_RANDOMEFFECTS_COVARIANCEMODEL_PRECISIONMODEL_H

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
				// PrecisionModel
				class PrecisionModel : public ICovarianceModel
				{
				private:
					// Fields
					ImmutableMatrix<double> R;

					// Implementation
					void Decompose(const ImmutableTriangularMatrix<double> & design_precision);
					Vector<double> CoefficientsUpdate(const ImmutableVector<double> & design_jacobian, const ImmutableVector<double> & beta) const;
					int UpdateComponentsImpl(const ImmutableVector<double> & beta, const Control & control);

				public:
					// Construction
					PrecisionModel(const ImmutableMatrix<double> & precision, const ImmutableVector<double> & theta);
					~PrecisionModel();

					// Properties
					TriangularMatrix<double> CoefficientCovariance() const;
				};
			}
		}
	}
}

#endif
