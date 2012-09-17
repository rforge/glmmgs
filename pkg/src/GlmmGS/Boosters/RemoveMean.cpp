#include "../Standard.h"
#include "RemoveMean.h"

namespace GlmmGS
{
	namespace Boosters
	{
		void RemoveMean::Reparameterize(Vector<double> & beta)
		{
			const double mean = Mean(beta);
			const int n = beta.Size();
			for (int k = 0; k < n; ++k)
				beta(k) -= mean;
		}
	}
}
