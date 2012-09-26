#ifndef GLMMGS_RESPONSES_BERNOULLIRESPONSE_H
#define GLMMGS_RESPONSES_BERNOULLIRESPONSE_H

#include "../Standard.h"
#include "IResponse.h"

namespace GlmmGS
{
	namespace Responses
	{
		// BernoulliResponse
		class BernoulliResponse : public IResponse
		{
		private:
			// Fields
			ImmutableVector<int> response;

		public:
			// Construction
			BernoulliResponse(const ImmutableVector<int> & response);
			~BernoulliResponse();

			// Properties
			int NumberOfObservations() const;

			// Methods
			void EvaluateWorkingWeightsAndValues(Vector<double> & weights, Vector<double> & values, const ImmutableVector<double> & eta) const;
		};
	}
}

#endif
