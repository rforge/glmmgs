#include "Standard.h"
#include "GlmmGS.h"
#include "Responses/IResponse.h"
#include "FixedEffects/IBlock.h"
#include "RandomEffects/IBlock.h"

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
	  	  iterations(3)
	{

	}

	// Properties
	// Methods
	void GlmmGS::Fit(const Control & control)
	{
		// Initialize working weights and values
		const int nrecords = this->response->NumberOfObservations();
		this->eta = Vector<double>(nrecords);
		this->working_weights = Vector<double>(nrecords);
		this->working_values = Vector<double>(nrecords);
		this->EvaluateWorkingWeightsAndValues();

		// Gauss-Seidel loop
		Set(this->iterations, 0);
		for (;;)
		{
			int updates = 0;
			for (int inner = 0; inner < control.maxiter.inner; ++inner)
			{
				// Update
				if (this->UpdateCoefficients(control) == 0)
					break;
				++updates;

				// Increase gs updates
				++this->iterations(0);
			}
			for (int inner = 0; inner < control.maxiter.inner; ++inner)
			{
				// Update
				if (this->UpdateCovarianceComponents(control) == 0)
					break;
				++updates;
				++this->iterations(1);
			}
			if (updates == 0)
				break;

			// Increase iterations
			++this->iterations(2);

			// Check number of iterations
			if (this->iterations(2) >= control.maxiter.outer)
			{
				throw MaxIterationsException();
				break;
			}

			// Print iterations
			if (control.verbose)
				Print("Iterations: %d\n", this->iterations(2));
		}
	}

	int GlmmGS::UpdateCoefficients(const Control & control)
	{
		int updates = 0;

		// Update fixed-effects
		for (int block = 0; block < this->fixed_effects.Size(); ++block)
		{
			updates += this->fixed_effects(block)->UpdateCoefficients(this->working_weights, this->working_values, control);
			this->EvaluateWorkingWeightsAndValues();
		}

		// Update random-effects
		for (int block = 0; block < this->random_effects.Size(); ++block)
		{
			updates += this->random_effects(block)->UpdateCoefficients(this->working_weights, this->working_values, control);
			this->EvaluateWorkingWeightsAndValues();
		}

		return updates;
	}

	int GlmmGS::UpdateCovarianceComponents(const Control & control)
	{
		int updates = 0;

		// Update covariance components
		for (int block = 0; block < this->random_effects.Size(); ++block)
			updates += this->random_effects(block)->UpdateCovarianceComponents(this->working_weights, this->working_values, control);

		return updates;
	}

	void GlmmGS::EvaluateWorkingWeightsAndValues()
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
