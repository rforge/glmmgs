#ifndef GLMMGS_RANDOMEFFECTS_STRATIFIED_COVARIANCEMODELS_SPARSEPRECISIONMODEL_H
#define GLMMGS_RANDOMEFFECTS_STRATIFIED_COVARIANCEMODELS_SPARSEPRECISIONMODEL_H

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
				// SparsePrecisionModel
				class SparsePrecisionModel : public ICovarianceModel
				{
				private:
					// Fields
					int nvars;
					const LDL::SparseMatrix<double> R;
					LDL::SparseCholeskyDecomposition beta_precision_chol;
					Boosters::RemoveWeightedMean remove_weighted_mean;

					// Coefficients
					void Decompose(const TriangularMatrix<Vector<double> > & precision);
					Vector<Vector<double> > CoefficientsUpdate(const Vector<Vector<double> > & jacobian, const Vector<Vector<double> > & beta) const;
					void ReparameterizeCoefficients(Vector<Vector<double> > & beta,
							const ImmutableVector<Pointer<Variables::IVariable> > & variables) const;

					// Components
					int UpdateComponentsImpl(const Vector<Vector<double> > & beta, const Control & control);

				public:
					// Construction
					SparsePrecisionModel(int nvars, const LDL::SparseMatrix<double> & R, const ImmutableMatrix<double> & S);
					~SparsePrecisionModel();
				};
			}
		}
	}
}

#endif
