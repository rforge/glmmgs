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

		void BinomialResponseSection::AddResponse(Vector<const int> values)
		{
			this->values = values;
		}

		void BinomialResponseSection::AddCounts(Vector<const int> counts)
		{
			this->counts= counts;
		}

		void BinomialResponseSection::EndResponse()
		{
			this->data->response.Reset(new(bl) GlmmGS::Responses::BinomialResponse(this->values, this->counts));
		}
	}
}
