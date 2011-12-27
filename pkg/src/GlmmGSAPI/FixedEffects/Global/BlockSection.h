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
				VectorBuilder<Pointer<GlmmGS::Variables::IVariable> > variables;

				// Implementation
				void AddIntercept();
				void AddCovariate(Vector<const int> values);
				void AddCovariate(Vector<const double> values);
				void EndBlock();

			public:
				// Construction
				BlockSection(GlmmGSAPI & api);
				~BlockSection();
			};
		}
	}
}

#endif
