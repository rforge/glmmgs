#include "../Utilities/Comparer.h"
#include "Controls.h"

namespace GlmmGS
{
	Controls::Controls(double relative_tolerance, double absolute_tolerance, int maxiter, bool verbose)
		: comparer(relative_tolerance, absolute_tolerance), maxiter(maxiter), verbose(verbose)
	{
	}
}
