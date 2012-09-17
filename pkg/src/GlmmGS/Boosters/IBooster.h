#ifndef GLMMGS_BOOSTERS_IBOOSTER_H
#define GLMMGS_BOOSTERS_IBOOSTER_H

#include "../Standard.h"

namespace GlmmGS
{
	namespace Boosters
	{
		class IBooster
		{
		public:
			// Construction
			IBooster();
			virtual ~IBooster();

			// Methods
			virtual void Reparameterize(Vector<double> & beta) = 0;
		};
	}
}

#endif
