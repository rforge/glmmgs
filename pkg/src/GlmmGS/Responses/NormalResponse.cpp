#include "../Standard.h"
#include "NormalResponse.h"

namespace GlmmGS
{
	namespace Responses
	{
		// BinomialResponse
		NormalResponse::NormalResponse(const ImmutableVector<double> & response)
			: response(response), dispersion(1.0)
		{
		}

		NormalResponse::~NormalResponse()
		{
		}

		int NormalResponse::NumberOfObservations() const
		{
			return this->response.Size();
		}

		void NormalResponse::EvaluateWorkingWeightsAndValues(Vector<double> & weights, Vector<double> & values, const ImmutableVector<double> & eta) const
		{
			_ASSERT_ARGUMENT(this->response.Size() == weights.Size() &&
				this->response.Size() == values.Size() &&
				this->response.Size() == eta.Size());

			const int nobs = this->response.Size();
			const double precision = 1.0 / this->dispersion;
			for (int i = 0; i < nobs; ++i)
			{
				values(i) = this->response(i) - eta(i); // First derivative in eta
				weights(i) = precision; // Minus the second derivative in eta
			}
		}
	}
}
