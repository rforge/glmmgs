#ifndef GLMMGS_RESPONSES_NORMALRESPONSE_H
#define GLMMGS_RESPONSES_NORMALRESPONSE_H

#include "../Standard.h"
#include "IResponse.h"

namespace GlmmGS
{
	namespace Responses
	{
		// NormalResponse
		class NormalResponse : public IResponse
		{
		private:
			// Fields
			ImmutableVector<double> response;
			double dispersion;

		public:
			// Construction
			NormalResponse(const ImmutableVector<double> & response);
			~NormalResponse();

			// Properties
			int NumberOfObservations() const;

			// Methods
			void EvaluateWorkingWeightsAndValues(Vector<double> & weights, Vector<double> & values, const ImmutableVector<double> & eta) const;
		};
	}
}

#endif
