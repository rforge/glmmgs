#pragma once

#include "../Standard.h"
#include "Working/IBlock.h"

namespace GlmmGS
{
	namespace FixedEffects
	{
		// IFixedEffectsBlock
		class IBlock
		{
		public:
			// Construction
			IBlock();
			virtual ~IBlock();

			// Properties
			virtual Pointer<Working::IBlock> CreateWorking() const = 0;
		};
	}
}
