#ifndef GLMMGS_GLMMGS_H
#define GLMMGS_GLMMGS_H

#include "Standard.h"
#include "Responses/IResponse.h"
#include "Offsets/IOffset.h"
#include "FixedEffects/IBlock.h"
#include "RandomEffects/IBlock.h"
#include "Control.h"

namespace GlmmGS
{
	// GlmmGS
	class GlmmGS
	{
	private:
		// Fields
		Pointer<Responses::IResponse> response;
		Pointer<Offsets::IOffset> offset;
		Vector<Pointer<FixedEffects::IBlock> > fixed_effects;
		Vector<Pointer<RandomEffects::IBlock> > random_effects;
		Vector<double> eta;
		Vector<double> working_weights;
		Vector<double> working_values;
		Vector<int> iterations;

		// Implementation
		int UpdateCoefficients(const Control & control);
		int UpdateCovarianceComponents(const Control & control);
		void EvaluateWorkingWeightsAndValues();

	public:
		// Construction
		GlmmGS(const Pointer<Responses::IResponse> & response,
				const Pointer<Offsets::IOffset> & offset,
				const Vector<Pointer<FixedEffects::IBlock> > & fixed_effects,
				const Vector<Pointer<RandomEffects::IBlock> > & random_effects);

		// Properties
		Vector<Pointer<FixedEffects::IBlock> > FixedEffects() const;
		Vector<Pointer<RandomEffects::IBlock> > RandomEffects() const;
		const ImmutableVector<int> & Iterations() const;

		// Methods
		void Fit(const Control & control);
	};

	inline
	Vector<Pointer<FixedEffects::IBlock> > GlmmGS::FixedEffects() const
	{
		return this->fixed_effects;
	}

	inline
	Vector<Pointer<RandomEffects::IBlock> > GlmmGS::RandomEffects() const
	{
		return this->random_effects;
	}

	inline
	const ImmutableVector<int> & GlmmGS::Iterations() const
	{
		return this->iterations;
	}
}

#endif
