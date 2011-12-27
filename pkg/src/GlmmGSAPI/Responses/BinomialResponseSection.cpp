#include "../Standard.h"
#include "BinomialResponseSection.h"
#include "../GlmmGSAPI.h"

namespace GlmmGSAPI
{
	namespace Responses
	{
		// BinomialResponseSection
		BinomialResponseSection::BinomialResponseSection(GlmmGSAPI & api)
			: Section(api)
		{
		}

		BinomialResponseSection::~BinomialResponseSection()
		{
		}

		void BinomialResponseSection::AddResponse(Vector<const int> values)
		{
			this->values.Reset(new(bl) Vector<const int>(values));
		}

		void BinomialResponseSection::AddCounts(Vector<const int> counts)
		{
			this->counts.Reset(new(bl) Vector<const int>(counts));
		}

		void BinomialResponseSection::EndResponse()
		{
			this->api.response.Reset(new(bl) GlmmGS::Responses::BinomialResponse(*this->values, *this->counts));
		}
	}
}
