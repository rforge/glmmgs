#ifndef GLMMGS_RANDOMEFFECTS_STRATIFIED_IBLOCK_H
#define GLMMGS_RANDOMEFFECTS_STRATIFIED_IBLOCK_H

#include "../../Standard.h"
#include "../../Variables/IVariable.h"
#include "../../FixedEffects/IBlock.h"
#include "../IBlock.h"
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
				Vector<Vector<Vector<double> > > projectors;
				Pointer<CovarianceModels::ICovarianceModel> covariance_model;

				// Implementation
				void EvaluateLinearDependencies(const Pointer<FixedEffects::IBlock> & fixef);
				void UpdatePredictor(Vector<double> & eta) const;
				int UpdateCoefficients(const ImmutableVector<double> & weights, const ImmutableVector<double> & values, const Control & control);
				int UpdateCovarianceComponents(const ImmutableVector<double> & weights, const ImmutableVector<double> & values, const Control & control);

			public:
				// Construction
				Block(const Vector<Pointer<Variables::IVariable> > & variables, WeakFactor factor,
						const Pointer<CovarianceModels::ICovarianceModel> & covariance_model);

				// Properties
				const Vector<Vector<double> > & Coefficients() const;
				Pointer<CovarianceModels::ICovarianceModel> CovarianceModel() const;
			};
		}
	}
}

#endif
