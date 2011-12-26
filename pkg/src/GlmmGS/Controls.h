#ifndef GLMMGS_CONTROLS_H
#define GLMMGS_CONTROLS_H

#include "../Utilities/Comparer.h"

namespace GlmmGS
{
	class Controls
	{
	private:
		// Fields
		Utilities::Comparer comparer;
		int maxiter;
		bool verbose;

	public:
		// Construction
		Controls(double relative_tolerance = 1.e-6, double absolute_tolerance = 1.e-6,
				int maxiter = 200, bool verbose = false);

		// Properties
		Utilities::Comparer Comparer() const;
		double Maxiter() const;
		bool Verbose() const;
	};

	inline
	Utilities::Comparer Controls::Comparer() const
	{
		return this->comparer;
	}

	inline
	double Controls::Maxiter() const
	{
		return this->maxiter;
	}

	inline
	bool Controls::Verbose() const
	{
		return this->verbose;
	}
}

#endif
