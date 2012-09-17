#ifndef GLMMGS_BOOSTERS_DEFAULT_H
#define GLMMGS_BOOSTERS_DEFAULT_H

#include "../Standard.h"
#include "IBooster.h"

namespace GlmmGS
{
	namespace Boosters
	{
		class Default : public IBooster
		{
		public:
			void Reparameterize(Vector<double> & beta);
		};
	}
}

#endif
