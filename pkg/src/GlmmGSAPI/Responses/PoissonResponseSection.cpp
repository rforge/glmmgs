#include "../Standard.h"
#include "PoissonResponseSection.h"
#include "../GlmmGSAPI.h"

namespace GlmmGSAPI
{
	namespace Responses
	{
		// PoissonResponseSection
		PoissonResponseSection::PoissonResponseSection(const Section & section)
			: Section(section)
		{

		}

		void PoissonResponseSection::AddResponse(const ImmutableVector<int> & values)
		{
			this->values = values;
		}

		void PoissonResponseSection::EndResponse()
		{
			this->data->response.Reset(new(bl) GlmmGS::Responses::PoissonResponse(this->values));
		}
	}
}
