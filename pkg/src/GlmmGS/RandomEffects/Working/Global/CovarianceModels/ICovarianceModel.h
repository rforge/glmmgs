#pragma once

#include "../../../../Standard.h"
#include "../../../../Estimate.h"

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
					// CovarianceModel
					class ICovarianceModel
					{
					public:
						// Construction
						ICovarianceModel();
						virtual ~ICovarianceModel();

						// Properties
						virtual Vector<Estimate> Estimates() const = 0;

						// Methods
						virtual void Decompose(const TriangularMatrix<double> & precision) = 0;
						virtual int Update(const Vector<double> & beta, Comparer comparer) = 0;
						virtual Vector<double> CoefficientsUpdate(const Vector<double> & jacobian, const Vector<double> & beta) const = 0;
					};
				}
			}
		}
	}
}
