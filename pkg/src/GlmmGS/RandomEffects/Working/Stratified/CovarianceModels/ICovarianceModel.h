#pragma once

#include "../../../../Standard.h"
#include "../../../../Estimate.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Working
		{
			namespace Stratified
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
						virtual void Decompose(const TriangularMatrix<Vector<double> > & precision) = 0;
						virtual int Update(const Vector<Vector<double> > & beta, Comparer comparer) = 0;
						virtual Vector<Vector<double> > CoefficientsUpdate(const Vector<Vector<double> > & jacobian, const Vector<Vector<double> > & beta) const = 0;
					};
				}
			}
		}
	}
}
