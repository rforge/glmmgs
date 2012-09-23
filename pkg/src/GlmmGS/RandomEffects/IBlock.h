#ifndef GLMMGS_RANDOMEFFECTS_IBLOCK_H
#define GLMMGS_RANDOMEFFECTS_IBLOCK_H

#include "../Standard.h"
#include "../Control.h"

namespace GlmmGS
{
	namespace RandomEffects
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
			virtual int UpdateCoefficients(const ImmutableVector<double> & weights, const ImmutableVector<double> & values, const Control & control) = 0;
			virtual int UpdateCovarianceComponents(const ImmutableVector<double> & weights, const ImmutableVector<double> & values, const Control & control) = 0;
		};
	}
}

#endif
