#include "ICovarianceModel.h"
#include "../../../Controls.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Global
		{
			namespace CovarianceModels
			{
				// Construction
				ICovarianceModel::ICovarianceModel(int npars, const ImmutableMatrix<double> & S)
					: theta(npars), constant(S.NumberOfRows() > 0)
				{
				}

				ICovarianceModel::~ICovarianceModel()
				{
				}

				// Properties
				const ImmutableVector<double> & ICovarianceModel::Components() const
				{
					return this->theta;
				}

				TriangularMatrix<double> ICovarianceModel::Covariance() const
				{
					return this->constant ? TriangularMatrix<double>(this->theta.Size()) : this->chol.Inverse();
				}

				TriangularMatrix<double> ICovarianceModel::CoefficientCovariance() const
				{
					return this->beta_precision_chol.Inverse();
				}

				// Implementation
				int ICovarianceModel::Update(const ImmutableTriangularMatrix<double> & minus_hessian,
						const ImmutableVector<double> & jacobian, const Controls & controls)
				{
					if (!this->constant)
					{
						// Calculate update
						this->chol.Decompose(minus_hessian);
						Vector<double> h = chol.Solve(jacobian);
						const int update = controls.Comparer().IsZero(h, this->theta) ? 0 : 1;

						// Scale updates
						const double max = MaxAbs(h);
						if (controls.Verbose())
							Print("Max update covariance components: %g\n", max);
						if (max > 1.0)
							h *= 1.0 / max;

						this->theta += h;
						return update;
					}

					return 0;
				}
			}
		}
	}
}
