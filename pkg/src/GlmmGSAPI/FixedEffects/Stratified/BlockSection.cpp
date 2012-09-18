#include "../../Standard.h"
#include "../../GlmmGSAPI.h"
#include "BlockSection.h"

namespace GlmmGSAPI
{
	namespace FixedEffects
	{
		namespace Stratified
		{
			// BlockSection
			BlockSection::BlockSection(const Section & section, WeakFactor factor)
				: Section(section), factor(factor)
			{
			}

			void BlockSection::AddIntercept(int duplicate)
			{
				this->data->fixed_intercept = true;
				typedef GlmmGS::Variables::Intercept T;
				this->variables.Add(Pointer<T>(new(bl) T(duplicate)));
			}

			void BlockSection::AddCovariate(Vector<const int> values, int duplicate)
			{
				typedef GlmmGS::Variables::VectorVariable<const int> T;
				this->variables.Add(Pointer<T>(new(bl) T(values, duplicate)));
			}

			void BlockSection::AddCovariate(Vector<const double> values, int duplicate)
			{
				typedef GlmmGS::Variables::VectorVariable<const double> T;
				this->variables.Add(Pointer<T>(new(bl) T(values, duplicate)));
			}

			void BlockSection::EndStratifiedBlock()
			{
				typedef GlmmGS::FixedEffects::Stratified::Block T;
				this->data->fixed_effects.Add(Pointer<T>(new(bl) T(this->variables.ToVector(), this->factor)));
			}
		}
	}
}
