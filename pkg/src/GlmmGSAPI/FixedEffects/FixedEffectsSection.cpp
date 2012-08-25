#include "../Standard.h"
#include "FixedEffectsSection.h"
#include "../GlmmGSAPI.h"
#include "Global/BlockSection.h"
#include "Stratified/BlockSection.h"

namespace GlmmGSAPI
{
	namespace FixedEffects
	{
		// FixedEffectsSection
		FixedEffectsSection::FixedEffectsSection(const Section & section)
			: Section(section)
		{

		}

		Pointer<Section> FixedEffectsSection::BeginBlock()
		{
			return Pointer<Section>(new(bl) Global::BlockSection(*this));
		}

		Pointer<Section> FixedEffectsSection::BeginStratifiedBlock(WeakFactor factor)
		{
			return Pointer<Section>(new(bl) Stratified::BlockSection(*this, factor));
		}
	
		void FixedEffectsSection::EndFixedEffects()
		{
			// Do nothing
		}
	}
}
