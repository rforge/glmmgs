#ifndef GLMMGS_RANDOMEFFECTS_GLOBAL_BLOCK_H
#define GLMMGS_RANDOMEFFECTS_GLOBAL_BLOCK_H

#include "../../Standard.h"
#include "../../Variables/IVariable.h"
#include "CovarianceModels/ICovarianceModel.h"
#include "../../Control.h"
#include "../IBlock.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Global
		{
			// Block
			class Block : public IBlock
			{
			private:
				// Fields
				Vector<Pointer<Variables::IVariable> > variables;
				Vector<double> beta;
				Pointer<CovarianceModels::ICovarianceModel> covariance_model;

				// Implementation
				void UpdatePredictor(Vector<double> & eta) const;
				int UpdateCoefficients(const ImmutableVector<double> & weights, const ImmutableVector<double> & values, const Control & control);
				int UpdateCovarianceComponents(const ImmutableVector<double> & weights, const ImmutableVector<double> & values, const Control & control);

			public:
				// Construction
				Block(const Vector<Pointer<Variables::IVariable> > & variables,
						const Pointer<CovarianceModels::ICovarianceModel> & covariance_model);

				// Properties
				const ImmutableVector<double> & Coefficients() const;
				Pointer<CovarianceModels::ICovarianceModel> CovarianceModel() const;
			};
		}
	}
}

#endif
