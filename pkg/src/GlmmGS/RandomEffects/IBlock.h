#ifndef GLMMGS_RANDOMEFFECTS_IBLOCK_H
#define GLMMGS_RANDOMEFFECTS_IBLOCK_H

#include "../Standard.h"
#include "../Estimate.h"
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

			// Properties
			virtual Vector<Estimate> Coefficients() const = 0;
			virtual Vector<Estimate> CovarianceComponents() const = 0;

			// Methods
			virtual void UpdatePredictor(Vector<double> & eta) const = 0;
			virtual int Update(const Vector<double> & w, const Vector<double> & z, const Controls & controls) = 0;
		};
	}
}

#endif
