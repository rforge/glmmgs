#ifndef GLMMGS_FIXEDEFFECTS_IBLOCK_H
#define GLMMGS_FIXEDEFFECTS_IBLOCK_H

#include "../Standard.h"
#include "../Control.h"

namespace GlmmGS
{
	namespace FixedEffects
	{
		// IBlock
		class IBlock
		{
		protected:
			// Construction
			IBlock();
		public:
			virtual ~IBlock();

			// Methods
			virtual void UpdatePredictor(Vector<double> & eta) const = 0;
			virtual int UpdateCoefficients(const ImmutableVector<double> & weight, const ImmutableVector<double> & values, const Control & control) = 0;
		};
	}
}

#endif
