#ifndef RESPONSES_GLMMGSAPI_RESPONSES_POISSONRESPONSESECTION_H
#define RESPONSES_GLMMGSAPI_RESPONSES_POISSONRESPONSESECTION_H

#include "../Standard.h"
#include "../Section.h"

namespace GlmmGSAPI
{
	namespace Responses
	{
		// PoissonResponseSection
		class PoissonResponseSection : public Section
		{
		private:
			// Fields
			ImmutableVector<int> values;

			// Implementation
			void AddResponse(const ImmutableVector<int> & values);
			void EndResponse();

		public:
			// Construction
			PoissonResponseSection(const Section & section);
		};
	}
}

#endif
