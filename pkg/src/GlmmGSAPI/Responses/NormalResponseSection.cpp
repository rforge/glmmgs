#include "../Standard.h"
#include "../GlmmGSAPI.h"
#include "NormalResponseSection.h"

namespace GlmmGSAPI
{
	namespace Responses
	{
		// NormalResponseSection
		NormalResponseSection::NormalResponseSection(const Section & section)
			: Section(section)
		{
		}

		void NormalResponseSection::AddResponse(const ImmutableVector<double> & values)
		{
			this->values = values;
		}

		void NormalResponseSection::EndResponse()
		{
			this->data->response.Reset(new(bl) GlmmGS::Responses::NormalResponse(this->values));
		}
	}
}
