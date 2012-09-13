#ifndef GLMMGS_FIXEDEFFECTS_IBLOCK_H
#define GLMMGS_FIXEDEFFECTS_IBLOCK_H

#include "../Standard.h"
#include "../Estimate.h"
#include "../Controls.h"

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
			virtual int UpdateCoefficients(const Vector<double> & w, const Vector<double> & z, const Controls & controls) = 0;
		};
	}
}

#endif
