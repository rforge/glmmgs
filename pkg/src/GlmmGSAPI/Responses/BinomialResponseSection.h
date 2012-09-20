#ifndef RESPONSES_GLMMGSAPI_RESPONSES_BINOMIALRESPONSESECTION_H
#define RESPONSES_GLMMGSAPI_RESPONSES_BINOMIALRESPONSESECTION_H

#include "../Standard.h"
#include "../Section.h"

namespace GlmmGSAPI
{
	namespace Responses
	{
		// BinomialResponseSection
		class BinomialResponseSection : public Section
		{
		private:
			// Fields
			ImmutableVector<int> values;
			ImmutableVector<int> counts;

			// Implementation
			void AddResponse(const ImmutableVector<int> & values);
			void AddCounts(const ImmutableVector<int> & values);
			void EndResponse();

		public:
			// Construction
			BinomialResponseSection(const Section & section);
		};
	}
}

#endif
