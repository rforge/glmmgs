#ifndef GLMMGS_RANDOMEFFECTS_STRATIFIED_COVARIANCEMODELS_ICOVARIANCEMODEL_H
#define GLMMGS_RANDOMEFFECTS_STRATIFIED_COVARIANCEMODELS_ICOVARIANCEMODEL_H

#include "../../../Standard.h"
#include "../../../Control.h"
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
					virtual int UpdateComponentsImpl(const Vector<Vector<double> > & beta, const Control & control) = 0;
					int UpdateComponents(const ImmutableTriangularMatrix<double> & minus_hessian,
							const ImmutableVector<double> & jacobian, const Control & control);
				public:
					// Construction
					ICovarianceModel(int npars, const ImmutableMatrix<double> & S);
					virtual ~ICovarianceModel();

					// Properties
					const ImmutableVector<double> & Components() const;
					TriangularMatrix<double> Covariance() const;

					// Coefficients
					virtual void Decompose(const TriangularMatrix<Vector<double> > & precision) = 0;
					virtual Vector<Vector<double> > CoefficientsUpdate(	const Vector<Vector<double> > & jacobian, const Vector<Vector<double> > & beta) const = 0;
					virtual void ReparameterizeCoefficients(Vector<Vector<double> > & beta, const ImmutableVector<Pointer<Variables::IVariable> > & variables) const = 0;

					// Covariance components
					int UpdateComponents(const Vector<Vector<double> > & beta, const Control & control);
				};
			}
		}
	}
}

#endif
