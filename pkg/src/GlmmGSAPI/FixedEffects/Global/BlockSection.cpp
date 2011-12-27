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
			BlockSection::BlockSection(GlmmGSAPI & api) : Section(api)
			{
			}

			BlockSection::~BlockSection()
			{
			}

			// Implementation
			void BlockSection::AddIntercept()
			{
				this->api.fixed_intercept = true;
				typedef GlmmGS::Variables::Intercept T;
				this->variables.Add(Pointer<T>(new(bl) T));
			}

			void BlockSection::AddCovariate(Vector<const int> values)
			{
				typedef GlmmGS::Variables::VectorVariable<const int> T;
				this->variables.Add(Pointer<T>(new(bl) T(values)));
			}

			void BlockSection::AddCovariate(Vector<const double> values)
			{
				typedef GlmmGS::Variables::VectorVariable<const double> T;
				this->variables.Add(Pointer<T>(new(bl) T(values)));
			}

			void BlockSection::EndBlock()
			{
				typedef GlmmGS::FixedEffects::Global::Block T;
				this->api.fixed_effects.Add(Pointer<T>(new(bl) T(this->variables.ToVector())));
			}
		}
	}
}
