#ifndef RESPONSES_GLMMGSAPI_RESPONSES_BERNOULLIRESPONSESECTION_H
#define RESPONSES_GLMMGSAPI_RESPONSES_BERNOULLIRESPONSESECTION_H

#include "../Standard.h"
#include "../Section.h"

namespace GlmmGSAPI
{
	namespace Responses
	{
		// BernoulliResponseSection
		class BernoulliResponseSection : public Section
		{
		private:
			// Fields
			ImmutableVector<int> values;

			// Implementation
			void AddResponse(const ImmutableVector<int> & values);
			void EndResponse();

		public:
			// Construction
			BernoulliResponseSection(const Section & section);
		};
	}
}

#endif
