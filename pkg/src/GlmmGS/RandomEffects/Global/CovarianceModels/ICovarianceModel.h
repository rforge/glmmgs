#ifndef GLMMGS_RANDOMEFFECTS_COVARIANCEMODEL_ICOVARIANCEMODEL_H
#define GLMMGS_RANDOMEFFECTS_COVARIANCEMODEL_ICOVARIANCEMODEL_H

#include "../../../Standard.h"
#include "../../../Control.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Global
		{
			namespace CovarianceModels
			{
				// CovarianceModel
				class ICovarianceModel
				{
				protected:
					// Fields
					Vector<double> theta;
					const bool constant;
					CholeskyDecomposition chol;
					CholeskyDecomposition beta_precision_chol;

					// Components
					virtual int UpdateComponentsImpl(const ImmutableVector<double> & beta, const Control & control) = 0;
					int UpdateComponents(const ImmutableTriangularMatrix<double> & minus_hessian, const ImmutableVector<double> & jacobian, const Control & control);
				public:
					// Construction
					ICovarianceModel(int npars, const ImmutableVector<double> & theta);
					virtual ~ICovarianceModel();

					// Properties
					const ImmutableVector<double> & Components() const;
					TriangularMatrix<double> Covariance() const;
					TriangularMatrix<double> CoefficientCovariance() const;

					// Coefficients
					virtual void Decompose(const ImmutableTriangularMatrix<double> & design_precision) = 0;
					virtual Vector<double> CoefficientsUpdate(const ImmutableVector<double> & design_jacobian, const ImmutableVector<double> & beta) const = 0;

					// Components
					virtual int UpdateComponents(const ImmutableVector<double> & beta, const Control & control);
				};
			}
		}
	}
}

#endif
