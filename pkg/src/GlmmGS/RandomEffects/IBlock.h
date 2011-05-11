#pragma once

#include "../Standard.h"
#include "Working/IBlock.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		// IBlock
		class IBlock
		{
		public:
			// Construction
			IBlock();
			virtual ~IBlock();

			// Methods
			virtual Pointer<Working::IBlock> CreateWorking() const = 0;
		};
	}
}
