#pragma once
#include "../Utilities/Comparer.h"

namespace GlmmGS
{
	class Controls
	{
	private:
		// Fields
		Utilities::Comparer comparer;
		int maxiter;

	public:
		// Construction
		Controls(double relative_tolerance, double absolute_tolerance, int maxiter);

		// Properties
		Utilities::Comparer Comparer() const;
		double Maxiter() const;
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
}
