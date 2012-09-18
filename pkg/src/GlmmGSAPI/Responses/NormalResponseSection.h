#ifndef RESPONSES_GLMMGSAPI_RESPONSES_NORMALRESPONSESECTION_H
#define RESPONSES_GLMMGSAPI_RESPONSES_NORMALRESPONSESECTION_H

#include "../Standard.h"
#include "../Section.h"

namespace GlmmGSAPI
{
	namespace Responses
	{
		// NormalResponseSection
		class NormalResponseSection : public Section
		{
		private:
			// Fields
			Vector<const double> values;

			// Implementation
			void AddResponse(Vector<const double> values);
			void EndResponse();

		public:
			// Construction
			NormalResponseSection(const Section & section);
		};
	}
}

#endif
