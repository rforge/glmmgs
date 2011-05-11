#pragma once

#include "../../../../Standard.h"
#include "../../../../Estimate.h"
#include "ICovarianceModel.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Working
		{
			namespace Global
			{
				namespace CovarianceModels
				{
					// IdentityModel
					class IdentityModel : public ICovarianceModel
					{
					private:
						// Fields
						int size;
						double tau;
						CholeskyDecomposition chol;

						// Implementation
						Vector<Estimate> Estimates() const;
						void Decompose(const TriangularMatrix<double> & precision);
						int Update(const Vector<double> & beta, Comparer comparer);
						Vector<double> CoefficientsUpdate(const Vector<double> & jacobian, const Vector<double> & beta) const;

					public:
						// Construction
						IdentityModel(int nvars);
						~IdentityModel();
					};
				}
			}
		}
	}
}
