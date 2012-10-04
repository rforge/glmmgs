#ifndef GLMMGSAPI_RANDOMEFFECTS_FIXEDEFFECTS_STRATIFIED_BLOCKSECTION_H
#define GLMMGSAPI_RANDOMEFFECTS_FIXEDEFFECTS_STRATIFIED_BLOCKSECTION_H

#include "../../Standard.h"
#include "../../Section.h"

namespace GlmmGSAPI
{
	namespace FixedEffects
	{
		namespace Stratified
		{
			// BlockSection
			class BlockSection : public Section
			{
			private:
				// Fields
				WeakFactor factor;
				Collections::VectorBuilder<Pointer<GlmmGS::Variables::IVariable> > variables;

				// Implementation
				void AddIntercept(int duplicate);
				void AddCovariate(const ImmutableVector<int> & values, int duplicate);
				void AddCovariate(const ImmutableVector<double> & values, int duplicate);
				void EndStratifiedBlock();
	
			public:
				// Construction
				BlockSection(const Section & section, WeakFactor factor);
			};
		}
	}
}

#endif
