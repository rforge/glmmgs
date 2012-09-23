#include "../Standard.h"
#include "Boosters.h"

namespace GlmmGS
{
	namespace Boosters
	{
		// RemoveFirstComponent
//		void RemoveFirstComponent::Reparameterize(Vector<double> & beta) const
//		{
//			const double x = beta(0);
//			beta(0) = 0.0;
//			for (int i = 1; i < beta.Size(); ++i)
//				beta(i) -= x;
//		}

		// RemoveMean
		void RemoveMean::Reparameterize(Vector<double> & beta) const
		{
			const double mean = Mean(beta);
			beta -= mean;
		}

		// RemoveWeightedMean
		void RemoveWeightedMean::Reparameterize(Vector<double> & beta) const
		{
			const double mean = ScalarProduct(beta, this->weights);
			beta -= mean;
		}
	}
}
