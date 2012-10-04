#ifndef GLMMGS_RANDOMEFFECTS_STRATIFIED_COVARIANCEMODELS_IDENTITYMODEL_H
#define GLMMGS_RANDOMEFFECTS_STRATIFIED_COVARIANCEMODELS_IDENTITYMODEL_H

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
				// IdentityModel
				class IdentityModel : public ICovarianceModel
				{
				private:
					// Fields
					int nvars;
					int nlevels;
					VectorialCholeskyDecomposition beta_precision_chol;
					Boosters::RemoveMean remove_mean;

					// Coefficients
					void Decompose(const ImmutableTriangularMatrix<Vector<double> > & design_precision);
					Vector<Vector<double> > CoefficientsUpdate(const ImmutableVector<Vector<double> > & design_jacobian, const ImmutableVector<Vector<double> > & beta) const;
					void ReparameterizeCoefficients(Vector<Vector<double> > & beta,
							const ImmutableVector<Pointer<Variables::IVariable> > & variables) const;

					// Components
					int UpdateComponentsImpl(const ImmutableVector<Vector<double> > & beta, const Control & control);

				public:
					// Construction
					IdentityModel(int nvars, int nlevels, const ImmutableVector<double> & theta);
					~IdentityModel();
				};
			}
		}
	}
}

#endif
