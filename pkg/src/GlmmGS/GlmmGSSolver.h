#ifndef GLMMGS_GLMMGSSOLVER_H
#define GLMMGS_GLMMGSSOLVER_H

#include "Standard.h"
#include "Responses/IResponse.h"
#include "Offsets/IOffset.h"
#include "FixedEffects/IBlock.h"
#include "RandomEffects/IBlock.h"
#include "Controls.h"

namespace GlmmGS
{
	// GlmmGSSolver
	class GlmmGSSolver
	{
	private:
		// Fields
		Pointer<Responses::IResponse> response;
		Pointer<Offsets::IOffset> offset;
		Vector<Pointer<FixedEffects::IBlock> > fixed_effects;
		Vector<Pointer<RandomEffects::IBlock> > random_effects;
		Controls controls;
		Vector<double> eta;
		Vector<double> working_weights;
		Vector<double> working_values;
		int iterations;

		// Implementation
		int Update();
		void EvaluateWorkingWeightsAndValues();

	public:
		// Construction
		GlmmGSSolver(const Pointer<Responses::IResponse> & response,
				const Pointer<Offsets::IOffset> & offset,
				const Vector<Pointer<FixedEffects::IBlock> > & fixed_effects,
				const Vector<Pointer<RandomEffects::IBlock> > & random_effects,
				const Controls & controls);

		// Methods
		void Fit();

		// Properties
		int Iterations() const;
	};

	inline
	int GlmmGSSolver::Iterations() const
	{
		return this->iterations;
	}
}

#endif
