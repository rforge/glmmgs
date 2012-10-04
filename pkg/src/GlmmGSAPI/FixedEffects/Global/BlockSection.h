#ifndef GLMMGSAPI_RANDOMEFFECTS_FIXEDEFFECTS_GLOBAL_BLOCKSECTION_H
#define GLMMGSAPI_RANDOMEFFECTS_FIXEDEFFECTS_GLOBAL_BLOCKSECTION_H

#include "../../Standard.h"
#include "../../Section.h"

namespace GlmmGSAPI
{
	namespace FixedEffects
	{
		namespace Global
		{
			// BlockSection
			class BlockSection : public Section
			{
			private:
				// Fields
				Collections::VectorBuilder<Pointer<GlmmGS::Variables::IVariable> > variables;

				// Implementation
				void AddIntercept(int duplicate);
				void AddCovariate(const ImmutableVector<int> & values, int duplicate);
				void AddCovariate(const ImmutableVector<double> & values, int duplicate);
				void EndBlock();

			public:
				// Construction
				BlockSection(const Section & section);
			};
		}
	}
}

#endif
