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

		void PoissonResponseSection::AddResponse(Vector<const int> values)
		{
			this->values.Reset(new(bl) Vector<const int>(values));
		}

		void PoissonResponseSection::EndResponse()
		{
			this->data->response.Reset(new(bl) GlmmGS::Responses::PoissonResponse(*this->values));
		}
	}
}
