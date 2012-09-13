#include "Standard.h"
#include "GlmmGS.h"
#include "Responses/IResponse.h"
#include "FixedEffects/IBlock.h"
#include "RandomEffects/IBlock.h"
#include "GlmmGSSolver.h"

namespace GlmmGS
{
	// Helper function. TODO: move
	void ToVector(Vector<Estimate> & y, const Vector<Vector<Estimate> > & x)
	{
		// Calculate size
		int size = 0;
		for (int i = 0; i < x.Size(); ++i)
			size += x(i).Size();

		// Copy vectors
		y = Vector<Estimate>(size);
		for (int index = 0, i = 0; i < x.Size(); ++i)
			for (int j = 0; j < x(i).Size(); ++j, ++index)
				y(index) = x(i)(j);
	}

	// GlmmGS
	GlmmGS::GlmmGS(const Pointer<Responses::IResponse> & response,
			const Pointer<Offsets::IOffset> & offset,
			const Vector<Pointer<FixedEffects::IBlock> > & fixed_effects,
			const Vector<Pointer<RandomEffects::IBlock> > & random_effects)
		: response(response),
		  offset(offset),
		  fixed_effects(fixed_effects),
		  random_effects(random_effects),
	  	  iterations(-1)
	{

	}

	// Properties
	// Methods
	void GlmmGS::Fit(const Controls & controls)
	{
		GlmmGSSolver solver(this->response, this->offset, this->fixed_effects, this->random_effects, controls);
		solver.Fit();
		this->iterations = solver.Iterations();
	}
}
