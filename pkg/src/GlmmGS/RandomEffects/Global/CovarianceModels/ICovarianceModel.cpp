#include "../../../Standard.h"
#include "../../../Control.h"
#include "ICovarianceModel.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Global
		{
			namespace CovarianceModels
			{
				// Construction
				ICovarianceModel::ICovarianceModel(int npars, const ImmutableVector<double> & theta)
					: theta(npars), constant(theta.Size() > 0)
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

				// Covariance components
				int ICovarianceModel::UpdateComponents(const ImmutableVector<double> & beta, const Control & control)
				{
					return this->constant ? 0 : this->UpdateComponentsImpl(beta, control);
				}

				int ICovarianceModel::UpdateComponents(const ImmutableTriangularMatrix<double> & minus_hessian,
						const ImmutableVector<double> & jacobian, const Control & control)
				{
					// Decomponse precision
					this->chol.Decompose(minus_hessian);

					// Calculate update
					Vector<double> h = chol.Solve(jacobian);

					// Constrain update
					ConstrainUpdate(h, this->theta, control.max_values.vcomp);

					// Check if update is significant
					const int update = control.comparer.IsZero(h, this->theta) ? 0 : 1;

					// Scale update
					ScaleUpdate(h, control.max_updates.vcomp);

					// Update
					this->theta += h;

					// Constrain value
					ConstrainValue(this->theta, control.max_values.vcomp);

					// Print
					if (control.verbose)
						Print("Max update variance components: %g\n", MaxAbs(h));

					return update;
				}
			}
		}
	}
}
