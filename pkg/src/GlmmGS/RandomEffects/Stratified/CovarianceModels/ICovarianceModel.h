#ifndef GLMMGS_RANDOMEFFECTS_STRATIFIED_COVARIANCEMODELS_ICOVARIANCEMODEL_H
#define GLMMGS_RANDOMEFFECTS_STRATIFIED_COVARIANCEMODELS_ICOVARIANCEMODEL_H

#include "../../../Standard.h"
#include "../../../Controls.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Stratified
		{
			namespace CovarianceModels
			{
				// CovarianceModel
				class ICovarianceModel
				{
				protected:
					// Fields
					Vector<double> theta;
					CholeskyDecomposition chol;

					// Implementation
					int Update(const TriangularMatrix<double> & minus_hessian,
							const Vector<double> & jacobian, const Controls & controls);
				public:
					// Construction
					ICovarianceModel(int npars);
					virtual ~ICovarianceModel();

					// Properties
					const Vector<double> & Components() const;
					TriangularMatrix<double> Covariance() const;

					// Methods
					virtual void Decompose(const TriangularMatrix<Vector<double> > & precision) = 0;
					virtual int Update(const Vector<Vector<double> > & beta, const Controls & controls) = 0;
					virtual Vector<Vector<double> > UpdateCoefficients(const Vector<Vector<double> > & jacobian,
							const Vector<Vector<double> > & beta) const = 0;
				};
			}
		}
	}
}

#endif
