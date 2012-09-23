#include "../../../Standard.h"
#include "../../../Boosters/Boosters.h"
#include "../../../Variables/IVariable.h"
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
				IdentityModel::IdentityModel(int nvars, int nlevels, const ImmutableMatrix<double> & S)
					: ICovarianceModel(nvars, S), nvars(nvars), nlevels(nlevels)
				{
					_VALIDATE_ARGUMENT(!this->constant || S.NumberOfRows() == nvars);
					for (int i = 0; i < nvars; ++i)
						this->theta(i) = this->constant ? 1.0 / S(i, i) : 1.0;
				}

				IdentityModel::~IdentityModel()
				{
				}

				// Coefficients
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

				Vector<Vector<double> > IdentityModel::CoefficientsUpdate(const Vector<Vector<double> > & jacobian, const Vector<Vector<double> > & beta) const
				{
					// Add diagonal terms
					Vector<Vector<double> > jac = jacobian;
					for (int i = 0; i < this->nvars; ++i)
						for (int k = 0; k < this->nlevels; ++k)
							jac(i)(k) -= this->theta(i) * beta(i)(k);

					// Decomposes
					return this->beta_precision_chol.Solve(jac);
				}

				void IdentityModel::ReparameterizeCoefficients(Vector<Vector<double> > & beta,
						const ImmutableVector<Pointer<Variables::IVariable> > & variables) const
				{
					Boosters::Reparameterize(beta, variables, this->remove_mean);
				}

				// Components
				int IdentityModel::UpdateComponentsImpl(const Vector<Vector<double> > & beta, const Control & control)
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
					return ICovarianceModel::UpdateComponents(minus_hessian, jac, control);
				}
			}
		}
	}
}
