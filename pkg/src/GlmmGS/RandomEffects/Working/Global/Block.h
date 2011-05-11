#pragma once

#include "../../../Standard.h"
#include "../../../Variables/IVariable.h"
#include "CovarianceModels/ICovarianceModel.h"
#include "../../../Estimate.h"
#include "../IBlock.h"
#include "Coefficients.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Working
		{
			namespace Global
			{
				// Block
				class Block : public IBlock
				{
				private:
					// Fields
					Vector<Pointer<Variables::IVariable> > variables;
					Coefficients beta;
					Pointer<CovarianceModels::ICovarianceModel> covariance_model;

					// Implementation
					Vector<Estimate> VarianceComponents() const;
					void UpdatePredictor(Vector<double> & eta) const;
					int Update(const Vector<double> & w, const Vector<double> & z, Comparer comparer);

				public:
					// Construction
					Block(Vector<Pointer<Variables::IVariable> > variables, Pointer<CovarianceModels::ICovarianceModel> covariance_model);
				};
			}
		}
	}
}
