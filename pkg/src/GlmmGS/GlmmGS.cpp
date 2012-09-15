#include "Standard.h"
#include "GlmmGS.h"
#include "Responses/IResponse.h"
#include "FixedEffects/IBlock.h"
#include "RandomEffects/IBlock.h"
#include "GlmmGSSolver.h"

namespace GlmmGS
{
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
