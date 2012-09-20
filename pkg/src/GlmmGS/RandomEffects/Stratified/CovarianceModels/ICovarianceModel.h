#ifndef GLMMGS_RANDOMEFFECTS_STRATIFIED_COVARIANCEMODELS_ICOVARIANCEMODEL_H
#define GLMMGS_RANDOMEFFECTS_STRATIFIED_COVARIANCEMODELS_ICOVARIANCEMODEL_H

#include "../../../Standard.h"
#include "../../../Controls.h"
#include "../../../Variables/IVariable.h"

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
					const bool constant;
					CholeskyDecomposition chol;

					// Implementation
					int Update(const TriangularMatrix<double> & minus_hessian,
							const ImmutableVector<double> & jacobian, const Controls & controls);
				public:
					// Construction
					ICovarianceModel(int npars, Matrix<const double> S);
					virtual ~ICovarianceModel();

					// Properties
					const ImmutableVector<double> & Components() const;
					TriangularMatrix<double> Covariance() const;

					// Methods
					virtual void Decompose(const TriangularMatrix<Vector<double> > & precision) = 0;
					virtual int Update(const Vector<Vector<double> > & beta, const Controls & controls) = 0;
					virtual Vector<Vector<double> > CoefficientsUpdate(	const Vector<Vector<double> > & jacobian,
							const Vector<Vector<double> > & beta) const = 0;
					virtual void ReparameterizeCoefficients(Vector<Vector<double> > & beta,
							const Vector<Pointer<Variables::IVariable> > variables) const = 0;
				};
			}
		}
	}
}

#endif
