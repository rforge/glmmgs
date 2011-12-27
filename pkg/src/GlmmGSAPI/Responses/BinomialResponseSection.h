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
			Pointer<Vector<const int> > values;
			Pointer<Vector<const int> > counts;

			// Implementation
			void AddResponse(Vector<const int> values);
			void AddCounts(Vector<const int> values);
			void EndResponse();

		public:
			// Construction
			BinomialResponseSection(GlmmGSAPI & api);
			~BinomialResponseSection();
		};
	}
}

#endif
