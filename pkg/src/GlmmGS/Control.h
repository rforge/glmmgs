#ifndef GLMMGS_CONTROL_H
#define GLMMGS_CONTROL_H

#include "../Utilities/Utilities.h"
#include "../Utilities/LinearAlgebra/LinearAlgebra.h"
#include "../Utilities/Math/Math.h"

namespace GlmmGS
{
	// Control
	struct Control
	{
		struct UpperBounds
		{
			const double fixef;
			const double ranef;
			const double vcomp;

			UpperBounds(double fixef,
					double ranef,
					double vcomp);
			UpperBounds(const double values[3]);
		};

		struct MaxIter
		{
			const int inner;
			const int outer;

			MaxIter(int inner, int outer);
			MaxIter(const int maxiter[2]);
		};

		const Utilities::Comparer comparer;
		const UpperBounds max_updates;
		const UpperBounds max_values;
		const MaxIter maxiter;
		const bool verbose;

		// Construction
		Control(const Utilities::Comparer & comparer = Utilities::Comparer(1.e-6, 1.e-6),
				const struct Control::UpperBounds & max_updates = Control::UpperBounds(0, 0, 0),
				const struct Control::UpperBounds & max_values = Control::UpperBounds(0, 0, 0),
				const Control::MaxIter & maxiter = Control::MaxIter(100, 100),
				bool verbose = false);
	};

	// Scale update
	template <class TYPE>
	void ScaleUpdate(Utilities::Vector<TYPE> & h, double max_update)
	{
		using namespace Utilities::LinearAlgebra;
		if (max_update > 0.0)
		{
			const double max = MaxAbs(h);
			if (max > max_update)
				h *= max_update / max;
		}
	}

	// Constrain update
	inline
	void ConstrainUpdate(double & h, double beta, double max_value)
	{
		using namespace Utilities::Math;
		if (Abs(beta) == max_value)
			h = 0;
	}

	template <class TYPE>
	void ConstrainUpdate(Utilities::Vector<TYPE> & h, const Utilities::ImmutableVector<TYPE> & beta, double max_value)
	{
		using namespace Utilities::LinearAlgebra;
		if (max_value > 0.0)
		{
			for (int i = 0; i < beta.Size(); ++i)
				ConstrainUpdate(h(i), beta(i), max_value);
		}
	}

	// Constrain value
	inline
	void ConstrainValue(double & beta, double max_value)
	{
		using namespace Utilities::Math;
		if (Abs(beta) > max_value)
			beta = max_value;
	}

	template <class TYPE>
	void ConstrainValue(Utilities::Vector<TYPE> & beta, double max_value)
	{
		using namespace Utilities::LinearAlgebra;
		if (max_value > 0.0)
		{
			for (int i = 0; i < beta.Size(); ++i)
				ConstrainValue(beta(i), max_value);
		}
	}
}

#endif
