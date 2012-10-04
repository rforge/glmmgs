#ifndef GLMMGS_RANDOMEFFECTS_STRATIFIED_COVARIANCEMODELS_PRECISIONMODEL_H
#define GLMMGS_RANDOMEFFECTS_STRATIFIED_COVARIANCEMODELS_PRECISIONMODEL_H

#include "../../../Standard.h"
#include "../../../Boosters/Boosters.h"
#include "../../../Variables/IVariable.h"
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
					ImmutableMatrix<double> R;
					CholeskyDecomposition beta_precision_chol;
					Boosters::RemoveWeightedMean remove_weighted_mean;

					// Coefficients
					void Decompose(const ImmutableTriangularMatrix<Vector<double> > & design_precision);
					Vector<Vector<double> > CoefficientsUpdate(const ImmutableVector<Vector<double> > & design_jacobian, const ImmutableVector<Vector<double> > & beta) const;
					void ReparameterizeCoefficients(Vector<Vector<double> > & beta,	const ImmutableVector<Pointer<Variables::IVariable> > & variables) const;

					// Covariance components
					int UpdateComponentsImpl(const ImmutableVector<Vector<double> > & beta, const Control & control);

				public:
					// Construction
					PrecisionModel(int nvars, const ImmutableMatrix<double> & R, const ImmutableVector<double> & theta);
					~PrecisionModel();
				};
			}
		}
	}
}

#endif
