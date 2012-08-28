#ifndef GLMMGS_RANDOMEFFECTS_STRATIFIED_BOOSTERS_IBOOSTER_H
#define GLMMGS_RANDOMEFFECTS_STRATIFIED_BOOSTERS_IBOOSTER_H

#include "../../../Standard.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Stratified
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
					virtual void Reparameterize(Vector<double> & intercept) = 0;
				};
			}
		}
	}
}

#endif
