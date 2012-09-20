#include "../Standard.h"
#include "BinomialResponseSection.h"
#include "../GlmmGSAPI.h"

namespace GlmmGSAPI
{
	namespace Responses
	{
		// BinomialResponseSection
		BinomialResponseSection::BinomialResponseSection(const Section & section)
			: Section(section)
		{
		}

		void BinomialResponseSection::AddResponse(const ImmutableVector<int> & values)
		{
			this->values = values;
		}

		void BinomialResponseSection::AddCounts(const ImmutableVector<int> & counts)
		{
			this->counts= counts;
		}

		void BinomialResponseSection::EndResponse()
		{
			this->data->response.Reset(new(bl) GlmmGS::Responses::BinomialResponse(this->values, this->counts));
		}
	}
}
