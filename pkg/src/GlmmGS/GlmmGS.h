#pragma once

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
		Controls controls;
		Vector<Estimate> beta;
		Vector<Estimate> theta;

	public:
		// Construction
		GlmmGS(Controls controls);

		// Properties
		void SetControls(Controls controls);
		const Vector<Estimate> & FixedEffectsCoefficients() const;
		const Vector<Estimate> & VarianceComponents() const;

		// Methods
		void Fit(const Pointer<Responses::IResponse> y, const Pointer<Offsets::IOffset> offset, const Vector<Pointer<FixedEffects::IBlock> > & x, const Vector<Pointer<RandomEffects::IBlock> > & z);
	};

	inline
	const Vector<Estimate> & GlmmGS::FixedEffectsCoefficients() const
	{
		return this->beta;
	}

	inline
	const Vector<Estimate> & GlmmGS::VarianceComponents() const
	{
		return this->theta;
	}
}
