#include "GlmmGSSolver.h"
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
	GlmmGSSolver::GlmmGSSolver()
	{
	}

	void GlmmGSSolver::Fit(const Pointer<Responses::IResponse> & y,
			const Pointer<Offsets::IOffset> & offset,
			const Vector<Pointer<FixedEffects::IBlock> > & x,
			const Vector<Pointer<RandomEffects::IBlock> > & z,
			const Controls & controls)
	{
		// Set response
		this->response = y;

		// Set offset
		this->offset = offset;

		// Set working fixed-effects
		this->fixed_effects = Vector<Pointer<FixedEffects::Working::IBlock> >(x.Size());
		for (int block = 0; block < x.Size(); ++block)
			this->fixed_effects(block) = x(block)->CreateWorking();

		// Set working random-effects working coefficients
		this->random_effects = Vector<Pointer<RandomEffects::Working::IBlock> >(z.Size());
		for (int block = 0; block < z.Size(); ++block)
			this->random_effects(block) = z(block)->CreateWorking();

		// Initialize working weights and values
		const int nrecords = y->NumberOfObservations();
		this->eta = Vector<double>(nrecords);
		this->working_weights = Vector<double>(nrecords);
		this->working_values = Vector<double>(nrecords);
		this->EvaluateWorkingWeightsAndValues();

		// Set controls
		this->controls = controls;

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

	// Properties
	Vector<Vector<Estimate> > GlmmGSSolver::FixedEffectsCoefficients() const
	{
		const int nblocks = this->fixed_effects.Size();
		Vector<Vector<Estimate> > estimates(nblocks);
		for (int block = 0; block < nblocks; ++block)
			estimates(block) = this->fixed_effects(block)->Coefficients();
		return estimates;
	}

	Vector<Vector<Estimate> > GlmmGSSolver::RandomEffectsCoefficients() const
	{
		const int nblocks = this->random_effects.Size();
		Vector<Vector<Estimate> > estimates(nblocks);
		for (int block = 0; block < nblocks; ++block)
			estimates(block) = this->random_effects(block)->Coefficients();
		return estimates;
	}

	Vector<Vector<Estimate> > GlmmGSSolver::CovarianceComponents() const
	{
		const int nblocks = this->random_effects.Size();
		Vector<Vector<Estimate> > estimates(nblocks);
		for (int block = 0; block < nblocks; ++block)
			estimates(block) = this->random_effects(block)->CovarianceComponents();
		return estimates;
	}
}
