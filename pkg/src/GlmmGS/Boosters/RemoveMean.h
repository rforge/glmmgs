#ifndef GLMMGS_BOOSTERS_REMOVEMEAN_H
#define GLMMGS_BOOSTERS_REMOVEMEAN_H

#include "../Standard.h"
#include "IBooster.h"

namespace GlmmGS
{
	namespace Boosters
	{
		class RemoveMean : public IBooster
		{
		public:
			void Reparameterize(Vector<double> & beta);
		};
	}
}

#endif
