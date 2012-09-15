#include "../../../Standard.h"
#include "Functions.h"
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
				PrecisionModel::PrecisionModel(Matrix<const double> R)
					: ICovarianceModel(1), R(R)
				{
					Set(this->theta, 1.0);
				}

				PrecisionModel::~PrecisionModel()
				{
				}

				// Methods
				void PrecisionModel::Decompose(const TriangularMatrix<double> & design_precision)
				{
					// Add diagonal to precision
					TriangularMatrix<double> prec = design_precision;
					const int size = this->R.NumberOfRows();
					for (int i = 0; i < size; ++i)
					{
						prec(i, i) += this->theta(0) * this->R(i, i);
						for (int j = 0; j < i; ++j)
							prec(i, j) += this->R(i, j);
					}

					// Decompose
					this->beta_precision_chol.Decompose(prec);
				}

				int PrecisionModel::Update(const Vector<double> & beta, const Controls & controls)
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
					const double bsquare = Square(this->R, beta);
					const double trace = Trace(a);
					jac(0) = ncols / this->theta(0) - bsquare - trace;
					minus_hessian(0, 0) = ncols / Math::Square(this->theta(0)) - SquareTrace(a);

					// Update covariance components
					return ICovarianceModel::Update(minus_hessian, jac, controls);
				}

				Vector<double> PrecisionModel::UpdateCoefficients(const Vector<double> & design_jacobian, const Vector<double> & beta) const
				{
					// Add diagonal terms
					const int size = this->R.NumberOfRows();
					Vector<double> jac(size);
					for (int i = 0; i < size; ++i)
						jac(i) = design_jacobian(i) - this->theta(0) * MatrixProduct(i, this->R, beta);

					// Solve
					return this->beta_precision_chol.Solve(jac);
				}
			}
		}
	}
}
