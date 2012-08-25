#ifndef GLMMGS_RANDOMEFFECTS_STRATIFIED_COVARIANCEMODELS_SPARSEPRECISIONMODEL_H
#define GLMMGS_RANDOMEFFECTS_STRATIFIED_COVARIANCEMODELS_SPARSEPRECISIONMODEL_H

#include "../../../Standard.h"
#include "../../../Estimate.h"
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

					// Implementation
					Vector<double> CoefficientsVariance() const;
					void Decompose(const TriangularMatrix<Vector<double> > & precision);
					int Update(const Vector<Vector<double> > & beta, const Controls & controls);
					Vector<Vector<double> > UpdateCoefficients(const Vector<Vector<double> > & jacobian, const Vector<Vector<double> > & beta) const;

				public:
					// Construction
					SparsePrecisionModel(int nvars, const LDL::SparseMatrix<double> & R);
					~SparsePrecisionModel();
				};
			}
		}
	}
}

#endif
