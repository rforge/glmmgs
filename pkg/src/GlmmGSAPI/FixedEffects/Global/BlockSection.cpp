#include "../../Standard.h"
#include "../../GlmmGSAPI.h"
#include "BlockSection.h"

namespace GlmmGSAPI
{
	namespace FixedEffects
	{
		namespace Global
		{
			// BlockSection
			BlockSection::BlockSection(const Section & section)
				: Section(section)
			{

			}

			void BlockSection::AddIntercept(int duplicate)
			{
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

			void BlockSection::EndBlock()
			{
				typedef GlmmGS::FixedEffects::Global::Block T;
				this->data->fixed_effects.Add(Pointer<T>(new(bl) T(this->variables.ToVector())));
			}
		}
	}
}
