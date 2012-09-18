#ifndef GLMMGS_BOOSTERS_IBOOSTER_H
#define GLMMGS_BOOSTERS_IBOOSTER_H

#include "../Standard.h"
#include "../Variables/IVariable.h"

namespace GlmmGS
{
	namespace Boosters
	{
		// RemoveFirstComponent
//		class RemoveFirstComponent
//		{
//		public:
//			void Reparameterize(Vector<double> & beta) const;
//		};

		// RemoveMean
		class RemoveMean
		{
		public:
			void Reparameterize(Vector<double> & beta) const;
		};

		// RemoveWeightedMean
		class RemoveWeightedMean
		{
		private:
			Vector<double> weights;
		public:
			template <class MATRIX>	RemoveWeightedMean(const MATRIX & R);
			void Reparameterize(Vector<double> & beta) const;
		};

		template <class MATRIX>
		RemoveWeightedMean::RemoveWeightedMean(const MATRIX & R)
			: weights(ColumnSums(R))
		{
			// Check sum of weights
			const double sum = Sum(this->weights);
			const double eps = 1.e-7;
			if (Abs(sum) < eps)
				Set(this->weights, 1.0);
			else
				this->weights *= 1.0 / sum;
		}

		// Reparameterize
		template <class BOOSTER>
		void Reparameterize(
				Vector<Vector<double> > & beta,
				const Vector<Pointer<Variables::IVariable> > variables,
				const BOOSTER & booster)
		{
			for (int j = 0; j < variables.Size(); ++j)
				if (variables(j)->duplicate == 1)
					booster.Reparameterize(beta(j));
		}
	}
}

#endif
