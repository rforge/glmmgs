#ifndef GLMMGS_GLMMGS_H
#define GLMMGS_GLMMGS_H

#include "Standard.h"
#include "Responses/IResponse.h"
#include "Offsets/IOffset.h"
#include "FixedEffects/IBlock.h"
#include "RandomEffects/IBlock.h"
#include "Controls.h"
#include "Estimate.h"

namespace GlmmGS
{
	// GlmmGS
	class GlmmGS
	{
		// Fields
		Pointer<Responses::IResponse> response;
		Pointer<Offsets::IOffset> offset;
		Vector<Pointer<FixedEffects::IBlock> > fixed_effects;
		Vector<Pointer<RandomEffects::IBlock> > random_effects;
		int iterations;

	public:
		// Construction
		GlmmGS(const Pointer<Responses::IResponse> & response,
				const Pointer<Offsets::IOffset> & offset,
				const Vector<Pointer<FixedEffects::IBlock> > & fixed_effects,
				const Vector<Pointer<RandomEffects::IBlock> > & random_effects);

		// Properties
		int Iterations() const;

		// Methods
		void Fit(const Controls & controls);
	};

	inline
	int GlmmGS::Iterations() const
	{
		return this->iterations;
	}
}

#endif
