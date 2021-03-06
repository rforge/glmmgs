#include "../../../Standard.h"
#include "../../../Boosters/Boosters.h"
#include "../../../Variables/IVariable.h"
#include "../../CovarianceModelUtilities.h"
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
				IdentityModel::IdentityModel(int nvars, int nlevels, const ImmutableVector<double> & theta)
					: ICovarianceModel(nvars, theta), nvars(nvars), nlevels(nlevels)
				{
					if (this->constant)
					{
						ValidateTheta(this->theta, theta);
						Copy(this->theta, theta);
					}
					else
					{
						Set(this->theta, 1.0);
					}

				}

				IdentityModel::~IdentityModel()
				{
				}

				// Coefficients
				void IdentityModel::Decompose(const ImmutableTriangularMatrix<Vector<double> > & design_precision)
				{
					// Add diagonal to precision
					TriangularMatrix<Vector<double> > prec(this->nvars);
					for (int i = 0; i < this->nvars; ++i)
					{
						// Strictly lower part
						for (int j = 0; j < i; ++j)
							prec(i, j) = Clone(design_precision(i, j));

						// Diagonal part
						prec(i, i) = Vector<double>(this->nlevels);
						for (int k = 0; k < this->nlevels; ++k)
							prec(i, i)(k) = design_precision(i, i)(k) + this->theta(i);
					}

					// Decompose
					this->beta_precision_chol.Decompose(prec);
				}

				Vector<Vector<double> > IdentityModel::CoefficientsUpdate(const ImmutableVector<Vector<double> > & design_jacobian,	const ImmutableVector<Vector<double> > & beta) const
				{
					// Add diagonal terms
					Vector<Vector<double> > jac(this->nvars);
					for (int i = 0; i < this->nvars; ++i)
					{
						jac(i) = Vector<double>(this->nlevels);
						for (int k = 0; k < this->nlevels; ++k)
							jac(i)(k) = design_jacobian(i)(k) - this->theta(i) * beta(i)(k);
					}

					// Decomposes
					return this->beta_precision_chol.Solve(jac);
				}

				void IdentityModel::ReparameterizeCoefficients(Vector<Vector<double> > & beta,
						const ImmutableVector<Pointer<Variables::IVariable> > & variables) const
				{
					Boosters::Reparameterize(beta, variables, this->remove_mean);
				}

				// Components
				int IdentityModel::UpdateComponentsImpl(const ImmutableVector<Vector<double> > & beta, const Control & control)
				{
					// Calculate variance
					const ImmutableTriangularMatrix<Vector<double> > covariance = this->beta_precision_chol.Inverse();

					// Calculate jacobian and minus the hessian
					Vector<double> jac(this->nvars);
					TriangularMatrix<double> minus_hessian(this->nvars);
					for (int i = 0; i < this->nvars; ++i)
					{
						const double trace = Sum(covariance(i, i));
						jac(i) = this->nlevels / this->theta(i) - LinearAlgebra::Square(beta(i)) - trace;
						minus_hessian(i, i) = this->nlevels / Math::Square(this->theta(i)) - LinearAlgebra::Square(covariance(i, i));
						for (int j = 0; j < i; ++j)
							minus_hessian(i, j) = -LinearAlgebra::Square(covariance(i, j));
					}

					// Update covariance components
					return ICovarianceModel::UpdateComponents(minus_hessian, jac, control);
				}
			}
		}
	}
}
