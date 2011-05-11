#pragma once

#include "../Standard.h"

namespace GlmmGS
{
	namespace Responses
	{
		// IResponse
		class IResponse
		{
		public:
			// Construction
			IResponse();
			virtual ~IResponse();

			// Properties
			virtual int NumberOfObservations() const = 0;

			// Methods
			virtual void EvaluateWorkingWeightsAndValues(Vector<double> & weights, Vector<double> & values, const Vector<double> & eta) const = 0;
		};
	}
}
