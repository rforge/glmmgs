#ifndef GLMMGS_RANDOMEFFECTS_STRATIFIED_BOOSTERS_DEFAULT_H
#define GLMMGS_RANDOMEFFECTS_STRATIFIED_BOOSTERS_DEFAULT_H

#include "../../../Standard.h"
#include "IBooster.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Stratified
		{
			namespace Boosters
			{
				class Default : public IBooster
				{
				public:
					void Reparameterize(Vector<double> & intercept);
				};
			}
		}
	}
}

#endif
