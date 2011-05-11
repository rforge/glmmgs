#pragma once

#include "../../Standard.h"
#include "../../Estimate.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Working
		{
			// IBlock
			class IBlock
			{
			public:
				// Construction
				IBlock();
				virtual ~IBlock();

				// Properties
				virtual Vector<Estimate> VarianceComponents() const = 0;

				// Methods
				virtual void UpdatePredictor(Vector<double> & eta) const = 0;
				virtual int Update(const Vector<double> & w, const Vector<double> & z, Comparer comparer) = 0;
			};
		}
	}
}
