#include "../Utilities/Utilities.h"
#include "Control.h"

namespace GlmmGS
{
	Control::UpperBounds::UpperBounds(double fixef,
			double ranef,
			double vcomp)
	: fixef(fixef),
	  ranef(ranef),
	  vcomp(vcomp)
	{

	}

	Control::UpperBounds::UpperBounds(const double values[3])
	: fixef(values[0]),
	  ranef(values[1]),
	  vcomp(values[2])
	{

	}

	Control::MaxIter::MaxIter(int inner, int outer)
	: inner(inner),
	  outer(outer)
	{

	}

	Control::MaxIter::MaxIter(const int maxiter[2])
	: inner(maxiter[0]),
	  outer(maxiter[1])
	{

	}

	Control::Control(const Utilities::Comparer & comparer,
			const Control::MaxIter & maxiter,
			bool verbose,
			const Control::UpperBounds & max_updates,
			const Control::UpperBounds & max_values)
	: comparer(comparer),
	  maxiter(maxiter),
	  verbose(verbose),
	  max_updates(max_updates),
	  max_values(max_values)
	{

	}
}
