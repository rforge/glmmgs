#ifndef GLMMGS_RANDOMEFFECTS_STRATIFIED_IBLOCK_H
#define GLMMGS_RANDOMEFFECTS_STRATIFIED_IBLOCK_H

#include "../../Standard.h"
#include "../../Variables/IVariable.h"
#include "../../Estimate.h"
#include "../IBlock.h"
#include "Boosters/IBooster.h"
#include "CovarianceModels/ICovarianceModel.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Stratified
		{
			// Block
			class Block : public IBlock
			{
			private:
				// Fields
				Vector<Pointer<Variables::IVariable> > variables;
				WeakFactor factor;
				Vector<Vector<double> > beta;
				Pointer<CovarianceModels::ICovarianceModel> covariance_model;
				Pointer<Boosters::IBooster> booster;

				// Implementation
				void UpdatePredictor(Vector<double> & eta) const;
				int Update(const Vector<double> & w, const Vector<double> & z, const Controls & controls);

			public:
				// Construction
				Block(const Vector<Pointer<Variables::IVariable> > & variables, WeakFactor factor,
						const Pointer<CovarianceModels::ICovarianceModel> & covariance_model,
						const Pointer<Boosters::IBooster> & booster);

				// Properties
				Vector<Vector<double> > Coefficients() const;
				Pointer<CovarianceModels::ICovarianceModel> CovarianceModel() const;
			};
		}
	}
}

#endif
