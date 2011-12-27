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
				VectorBuilder<Pointer<GlmmGS::Variables::IVariable> > variables;

				// Implementation
				void AddIntercept();
				void AddCovariate(Vector<const int> values);
				void AddCovariate(Vector<const double> values);
				void EndStratifiedBlock();
	
			public:
				// Construction
				BlockSection(GlmmGSAPI & api, WeakFactor factor);
				~BlockSection();
			};
		}
	}
}

#endif
