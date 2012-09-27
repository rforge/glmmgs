#include "../../../Standard.h"
#include "../../CovarianceModelFunctions.h"
#include "PrecisionModel.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Global
		{
			namespace CovarianceModels
			{
				// Construction
				PrecisionModel::PrecisionModel(const ImmutableMatrix<double> & R, const ImmutableMatrix<double> & S)
					: ICovarianceModel(1, S), R(R)
				{
					_VALIDATE_ARGUMENT(!this->constant || S.NumberOfRows() == 1);
					this->theta(0) = this->constant ? 1.0 / S(0, 0) : 1.0;
				}

				PrecisionModel::~PrecisionModel()
				{
				}

				// Coefficients
				void PrecisionModel::Decompose(const ImmutableTriangularMatrix<double> & design_precision)
				{
					// Add diagonal to precision
					const int size = this->R.NumberOfRows();
					TriangularMatrix<double> prec(size);
					for (int i = 0; i < size; ++i)
					{
						prec(i, i) = design_precision(i, i) + this->theta(0) * this->R(i, i);
						for (int j = 0; j < i; ++j)
							prec(i, j) = design_precision(i, j) + this->R(i, j);
					}

					// Decompose
					this->beta_precision_chol.Decompose(prec);
				}

				Vector<double> PrecisionModel::CoefficientsUpdate(const ImmutableVector<double> & design_jacobian, const ImmutableVector<double> & beta) const
				{
					// Add diagonal terms
					const int size = this->R.NumberOfRows();
					Vector<double> jacobian(size);
					for (int i = 0; i < size; ++i)
						jacobian(i) = design_jacobian(i) - this->theta(0) * MatrixProduct(i, this->R, beta);

					// Solve
					return this->beta_precision_chol.Solve(jacobian);
				}

				// Components
				int PrecisionModel::UpdateComponentsImpl(const ImmutableVector<double> & beta, const Control & control)
				{
					// Calulate T^{-1} R
					const int ncols = this->R.NumberOfColumns();
					Matrix<double> a(ncols, ncols);
					Vector<double> b(ncols);
					for (int j = 0; j < ncols; ++j)
					{
						// Prepare b
						for (int i = 0; i < ncols; ++i)
							b(i) = this->R(i, j);

						// Solve T x = b
						Vector<double> x = this->beta_precision_chol.Solve(b);

						// Store x
						for (int i = 0; i < ncols; ++i)
							a(i, j) = x(i);
					}

					// Calculate jacobian and minus the hessian
					Vector<double> jac(1);
					TriangularMatrix<double> minus_hessian(1);
					jac(0) = ncols / this->theta(0) - Square(this->R, beta) - Trace(a);
					minus_hessian(0, 0) = ncols / Math::Square(this->theta(0)) - SquareTrace(a);

					// Update covariance components
					return ICovarianceModel::UpdateComponents(minus_hessian, jac, control);
				}
			}
		}
	}
}
