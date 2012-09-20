#ifndef GLMMGS_RESPONSES_BINOMIALRESPONSE_H
#define GLMMGS_RESPONSES_BINOMIALRESPONSE_H

#include "../Standard.h"
#include "IResponse.h"

namespace GlmmGS
{
	namespace Responses
	{
		// BinomialResponse
		class BinomialResponse : public IResponse
		{
		private:
			// Fields
			ImmutableVector<int> response;
			ImmutableVector<int> counts;

		public:
			// Construction
			BinomialResponse(const ImmutableVector<int> & response, const ImmutableVector<int> & counts);
			~BinomialResponse();

			// Properties
			int NumberOfObservations() const;

			// Methods
			void EvaluateWorkingWeightsAndValues(Vector<double> & weights, Vector<double> & values, const ImmutableVector<double> & eta) const;
		};
	}
}

#endif
