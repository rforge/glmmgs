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

		void NormalResponseSection::AddResponse(Vector<const double> values)
		{
			this->values = values;
		}

		void NormalResponseSection::EndResponse()
		{
			this->data->response.Reset(new(bl) GlmmGS::Responses::NormalResponse(this->values));
		}
	}
}
