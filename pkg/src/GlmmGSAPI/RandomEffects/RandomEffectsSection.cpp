#include "../Standard.h"
#include "RandomEffectsSection.h"
#include "../GlmmGSAPI.h"
#include "Global/BlockSection.h"
#include "Stratified/BlockSection.h"

namespace GlmmGSAPI
{
	namespace RandomEffects
	{
		// RandomEffectsSection
		RandomEffectsSection::RandomEffectsSection(Section & section)
			: Section(section)
		{

		}

		Pointer<Section> RandomEffectsSection::BeginBlock()
		{
			return Pointer<Section>(new(bl) Global::BlockSection(*this));
		}

		Pointer<Section> RandomEffectsSection::BeginStratifiedBlock(WeakFactor factor)
		{
			return Pointer<Section>(new(bl) Stratified::BlockSection(*this, factor));
		}
	
		void RandomEffectsSection::EndRandomEffects()
		{
			// Do nothing
		}
	}
}
