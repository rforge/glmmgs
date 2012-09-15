#include "GlmmGSSolver.h"
#include "Responses/IResponse.h"
#include "FixedEffects/IBlock.h"
#include "RandomEffects/IBlock.h"
#include "Controls.h"

namespace GlmmGS
{
	// GlmmGSSolver
	GlmmGSSolver::GlmmGSSolver(const Pointer<Responses::IResponse> & response,
			const Pointer<Offsets::IOffset> & offset,
			const Vector<Pointer<FixedEffects::IBlock> > & fixed_effects,
			const Vector<Pointer<RandomEffects::IBlock> > & random_effects,
			const Controls & controls)
		: response(response),
		  offset(offset),
		  fixed_effects(fixed_effects),
		  random_effects(random_effects),
		  controls(controls),
		  iterations(-1)
	{

	}

	void GlmmGSSolver::Fit()
	{
		// Initialize working weights and values
		const int nrecords = this->response->NumberOfObservations();
		this->eta = Vector<double>(nrecords);
		this->working_weights = Vector<double>(nrecords);
		this->working_values = Vector<double>(nrecords);
		this->EvaluateWorkingWeightsAndValues();

		// Gauss-Seidel loop
		this->iterations = 0;
		for (;;)
		{
			// Update
			if (this->Update() == 0)
				break;

			// Increase counter
			++this->iterations;

			// Check number of iterations
			if (this->iterations >= this->controls.Maxiter())
			{
				throw MaxIterationsException();
				break;
			}

			// Print iterations
			if (this->controls.Verbose())
				Print("Iterations: %d\n", this->iterations);
		}
	}

	int GlmmGSSolver::Update()
	{
		int update = 0;

		// Update fixed-effects
		for (int block = 0; block < this->fixed_effects.Size(); ++block)
		{
			update += this->fixed_effects(block)->UpdateCoefficients(this->working_weights, this->working_values, this->controls);
			this->EvaluateWorkingWeightsAndValues();
		}

		// Update random-effects
		for (int block = 0; block < this->random_effects.Size(); ++block)
		{
			update += this->random_effects(block)->Update(this->working_weights, this->working_values, this->controls);
			this->EvaluateWorkingWeightsAndValues();
		}

		return update;
	}

	void GlmmGSSolver::EvaluateWorkingWeightsAndValues()
	{
		// Evaluate predictor
		this->offset->InitializePredictor(this->eta);
		for (int block = 0; block < this->fixed_effects.Size(); ++block)
			this->fixed_effects(block)->UpdatePredictor(this->eta);
		for (int block = 0; block < this->random_effects.Size(); ++block)
			this->random_effects(block)->UpdatePredictor(this->eta);

		// Evaluate working weights and values
		this->response->EvaluateWorkingWeightsAndValues(this->working_weights, this->working_values, this->eta);
	}
}
