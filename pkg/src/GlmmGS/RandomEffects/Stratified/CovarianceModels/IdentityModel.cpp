#include "../../../Standard.h"
#include "IdentityModel.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Stratified
		{
			namespace CovarianceModels
			{
				// Construction
				IdentityModel::IdentityModel(int nvars, int nlevels)
					: ICovarianceModel(nvars), nvars(nvars), nlevels(nlevels)
				{
					Set(this->theta, 1.0);
				}

				IdentityModel::~IdentityModel()
				{
				}

				// Implementation
				void IdentityModel::Decompose(const TriangularMatrix<Vector<double> > & design_precision)
				{
					// Add diagonal to precision
					TriangularMatrix<Vector<double> > prec = design_precision;
					for (int i = 0; i < this->nvars; ++i)
						for (int k = 0; k < this->nlevels; ++k)
							prec(i, i)(k) += this->theta(i);

					// Decompose
					this->beta_precision_chol.Decompose(prec);
				}

				int IdentityModel::Update(const Vector<Vector<double> > & beta, const Controls & controls)
				{
					// Calculate variance
					const TriangularMatrix<Vector<double> > covariance = this->beta_precision_chol.Inverse();

					// Calculate jacobian and minus the hessian
					Vector<double> jac(this->nvars);
					TriangularMatrix<double> minus_hessian(this->nvars);
					for (int i = 0; i < this->nvars; ++i)
					{
						const double bsquare = Square(beta(i));
						const double trace = Sum(covariance(i, i));
						jac(i) = this->nlevels / this->theta(i) - bsquare - trace;
						minus_hessian(i, i) = this->nlevels / Square(this->theta(i)) - Square(covariance(i, i));
						for (int j = 0; j < i; ++j)
							minus_hessian(i, j) = -Square(covariance(i, j));
					}

					// Update covariance components
					return ICovarianceModel::Update(minus_hessian, jac, controls);
				}

				Vector<Vector<double> > IdentityModel::UpdateCoefficients(const Vector<Vector<double> > & jacobian, const Vector<Vector<double> > & beta) const
				{
					// Add diagonal terms
					Vector<Vector<double> > jac = jacobian;
					for (int i = 0; i < this->nvars; ++i)
						for (int k = 0; k < this->nlevels; ++k)
							jac(i)(k) -= this->theta(i) * beta(i)(k);

					// Decomposes
					return this->beta_precision_chol.Solve(jac);
				}
			}
		}
	}
}
