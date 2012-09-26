#include "../Standard.h"
#include "BernoulliResponseSection.h"
#include "../GlmmGSAPI.h"

namespace GlmmGSAPI
{
	namespace Responses
	{
		// BinomialResponseSection
		BernoulliResponseSection::BernoulliResponseSection(const Section & section)
			: Section(section)
		{
		}

		void BernoulliResponseSection::AddResponse(const ImmutableVector<int> & values)
		{
			this->values = values;
		}

		void BernoulliResponseSection::EndResponse()
		{
			this->data->response.Reset(new(bl) GlmmGS::Responses::BernoulliResponse(this->values));
		}
	}
}
