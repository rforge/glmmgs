#include "../Standard.h"
#include "BernoulliResponse.h"

namespace GlmmGS
{
	namespace Responses
	{
		// BinomialResponse
		BernoulliResponse::BernoulliResponse(const ImmutableVector<int> & response)
			: response(response)
		{
		}

		BernoulliResponse::~BernoulliResponse()
		{
		}

		int BernoulliResponse::NumberOfObservations() const
		{
			return this->response.Size();
		}

		void BernoulliResponse::EvaluateWorkingWeightsAndValues(Vector<double> & weights, Vector<double> & values, const ImmutableVector<double> & eta) const
		{
			_ASSERT(this->response.Size() == weights.Size() &&
				this->response.Size() == values.Size() &&
				this->response.Size() == eta.Size());

			const int nobs = this->response.Size();

			for (int i = 0; i < nobs; ++i)
			{
				const double pi = AntiLogit(eta(i));
				values(i) = this->response(i) - pi; // First derivative in eta
				weights(i) = pi * (1.0 - pi); // Minus the second derivative in eta
			}
		}
	}
}
