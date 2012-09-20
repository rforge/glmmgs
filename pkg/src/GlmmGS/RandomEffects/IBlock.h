#ifndef GLMMGS_RANDOMEFFECTS_IBLOCK_H
#define GLMMGS_RANDOMEFFECTS_IBLOCK_H

#include "../Standard.h"
#include "../Controls.h"

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
			virtual int Update(const ImmutableVector<double> & w, const ImmutableVector<double> & z, const Controls & controls) = 0;
		};
	}
}

#endif
