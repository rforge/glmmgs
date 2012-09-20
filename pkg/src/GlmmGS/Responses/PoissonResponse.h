#ifndef GLMMGS_RESPONSES_POISSONRESPONSE_H
#define GLMMGS_RESPONSES_POISSONRESPONSE_H

#include "../Standard.h"
#include "IResponse.h"

namespace GlmmGS
{
	namespace Responses
	{
		// PoissonResponse
		class PoissonResponse : public IResponse
		{
		private:
			// Fields
			ImmutableVector<int> response;

		public:
			// Construction
			PoissonResponse(const ImmutableVector<int> & response);
			~PoissonResponse();

			// Properties
			int NumberOfObservations() const;

			// Methods
			void EvaluateWorkingWeightsAndValues(Vector<double> & weights, Vector<double> & values, const ImmutableVector<double> & eta) const;
		};
	}
}

#endif
