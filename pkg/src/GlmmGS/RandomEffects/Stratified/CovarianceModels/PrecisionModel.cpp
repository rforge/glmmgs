#include "../../../Standard.h"
#include "../../../Boosters/Boosters.h"
#include "../../../Variables/IVariable.h"
#include "../../CovarianceModelFunctions.h"
#include "PrecisionModel.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Stratified
		{
			namespace CovarianceModels
			{
				// Construction
				PrecisionModel::PrecisionModel(int nvars, const ImmutableMatrix<double> & R, const ImmutableMatrix<double> & S)
					: ICovarianceModel(nvars, S), nvars(nvars), R(R), remove_weighted_mean(R)
				{
					_VALIDATE_ARGUMENT(!this->constant || S.NumberOfRows() == nvars);
					for (int i = 0; i < nvars; ++i)
						this->theta(i) = this->constant ? 1.0 / S(i, i) : 1.0;
				}

				PrecisionModel::~PrecisionModel()
				{
				}

				// Coefficients
				void PrecisionModel::Decompose(const TriangularMatrix<Vector<double> > & design_precision)
				{
					// Add diagonal to precision
					const int nlevels = this->R.NumberOfRows();
					TriangularMatrix<double> prec(this->nvars * nlevels);
					for (int i = 0; i < this->nvars; ++i)
					{
						const int offset_i = i * nlevels;

						// Diagonal block
						for (int k = 0; k < nlevels; k++)
						{
							prec(offset_i + k, offset_i + k) = design_precision(i, i)(k) + this->theta(i) * this->R(k, k);
							for (int l = 0; l < k; ++l)
								prec(offset_i + k, offset_i + l) = this->theta(i) * this->R(k, l);
						}

						// Off-diagonal blocks
						for (int j = 0; j < i; ++j)
						{
							const int offset_j = j * nlevels;
							for (int k = 0; k < nlevels; k++)
								prec(offset_i + k, offset_j + k) += design_precision(i, j)(k);
						}
					}

					// Decompose
					this->beta_precision_chol.Decompose(prec);
				}

				Vector<Vector<double> > PrecisionModel::CoefficientsUpdate(const Vector<Vector<double> > & design_jacobian, const Vector<Vector<double> > & beta) const
				{
					// Add diagonal terms
					const int nlevels = this->R.NumberOfRows();
					Vector<double> jac(nlevels * nvars);
					for (int index = 0, i = 0; i < nvars; ++i)
						for (int k = 0; k < nlevels; ++k, ++index)
							jac(index) = design_jacobian(i)(k) - theta(i) * MatrixProduct(k, this->R, beta(i));

					// Solve
					Vector<double> h_tmp = this->beta_precision_chol.Solve(jac);
					Vector<Vector<double> > h(this->nvars);
					for (int index = 0, i = 0; i < this->nvars; ++i)
					{
						h(i) = Vector<double>(nlevels);
						for (int k = 0; k < nlevels; ++k, ++index)
							h(i)(k) = h_tmp(index);
					}
					return h;
				}

				void PrecisionModel::ReparameterizeCoefficients(Vector<Vector<double> > & beta,
						const ImmutableVector<Pointer<Variables::IVariable> > & variables) const
				{
					Boosters::Reparameterize(beta, variables, this->remove_weighted_mean);
				}

				// Components
				int PrecisionModel::UpdateComponentsImpl(const Vector<Vector<double> > & beta, const Control & control)
				{
					// Calulate T^{-1} R
					const int nlevels = this->R.NumberOfColumns();
					const int size = this->nvars * nlevels;
					Matrix<double> a(size, size);
					Vector<double> b(size);
					for (int i = 0, ik = 0; i < this->nvars; ++i)
					{
						for (int k = 0; k < nlevels; ++k, ++ik)
						{
							// Prepare b
							Set(b, 0.0);
							const int offset = i * nlevels;
							for (int l = 0; l < nlevels; ++l)
								b(offset + l) = this->R(l, k);

							// Solve T_j x = b
							Vector<double> x = this->beta_precision_chol.Solve(b);

							// Store x
							for (int j = 0; j < size; ++j)
								a(j, ik) = x(j);
						}
					}

					// Calculate jacobian and minus the hessian
					Vector<double> jac(this->nvars);
					TriangularMatrix<double> minus_hessian(this->nvars);
					for (int i = 0; i < this->nvars; ++i)
					{
						jac(i) = nlevels / this->theta(i) - Square(this->R, beta(i)) - BlockTrace(i, nlevels, a);
						minus_hessian(i, i) = nlevels / Math::Square(this->theta(i)) - BlockSquareTrace(i, i, nlevels, a);
						for (int j = 0; j < i; ++j)
							minus_hessian(i, j) = -BlockSquareTrace(i, j, nlevels, a);
					}

					// Update covariance components
					return ICovarianceModel::UpdateComponents(minus_hessian, jac, control);
				}
			}
		}
	}
}
