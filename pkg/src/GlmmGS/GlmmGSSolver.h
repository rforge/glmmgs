#pragma once

#include "Standard.h"
#include "Responses/IResponse.h"
#include "FixedEffects/IBlock.h"
#include "FixedEffects/Working/IBlock.h"
#include "RandomEffects/IBlock.h"
#include "RandomEffects/Working/IBlock.h"
#include "Estimate.h"
#include "Controls.h"

namespace GlmmGS
{
	// GlmmGSSolver
	class GlmmGSSolver
	{
	private:
		// Fields
		Controls controls;
		Pointer<Responses::IResponse> response;
		Vector<Pointer<FixedEffects::Working::IBlock> > fixed_effects;
		Vector<Pointer<RandomEffects::Working::IBlock> > random_effects;
		Vector<double> eta;
		Vector<double> working_weights;
		Vector<double> working_values;

		// Implementation
		int Update();
		void EvaluateWorkingWeightsAndValues();

	public:
		// Construction
		GlmmGSSolver(Controls controls);

		// Methods
		void Fit(const Pointer<Responses::IResponse> y, const Vector<Pointer<FixedEffects::IBlock> > & x, const Vector<Pointer<RandomEffects::IBlock> > & z);

		// Properties
		Vector<Vector<Estimate> > FixedEffectsCoefficients() const;
		Vector<Vector<Estimate> > VarianceComponents() const;
	};
}
