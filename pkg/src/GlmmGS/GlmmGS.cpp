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


		// Initialize counters
		Set(this->iterations, 0);

		// Fit initial coefficients
		const double maxit = Max(control.maxiter.outer, control.maxiter.inner);
		for (;;)
		{
			// Increase coefficient updates
			if (this->UpdateCoefficients(control) == 0)
				break;
			++this->iterations(1);

			// Check number of iterations
			if (this->iterations(1) >= maxit)
			{
				throw MaxIterationsException();
				break;
			}
		}

		// Update variance components and coefficients
		int updates = 0;
		for (;;)
		{
			// Update variance components
			for (int inner = 0; inner < control.maxiter.inner; ++inner)
			{
				if (this->UpdateCovarianceComponents(control) == 0)
					break;
				++updates;
				++this->iterations(2);
			}

			// Check and reset updates
			if (updates == 0)
				break;
			updates = 0;

			// Increase outer iterations
			++this->iterations(0);

			// Check number of iterations
			if (this->iterations(0) >= control.maxiter.outer)
			{
				throw MaxIterationsException();
				break;
			}

			// Print iterations
			if (control.verbose)
				Print("Iterations: %d (%d, %d)\n", this->iterations(0), this->iterations(1), this->iterations(2));

			// Update coefficients
			for (int inner = 0; inner < control.maxiter.inner; ++inner)
			{
				if (this->UpdateCoefficients(control) == 0)
					break;
				++updates;
				++this->iterations(1);
			}
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
