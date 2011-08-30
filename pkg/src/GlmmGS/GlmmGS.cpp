#include "Standard.h"
#include "GlmmGS.h"
#include "Responses/IResponse.h"
#include "FixedEffects/IBlock.h"
#include "RandomEffects/IBlock.h"
#include "GlmmGSSolver.h"

namespace GlmmGS
{
	// Helper function. TODO: move
	void ToVector(Vector<Estimate> & y, const Vector<Vector<Estimate> > & x)
	{
		// Calculate size
		int size = 0;
		for (int i = 0; i < x.Size(); ++i)
			size += x(i).Size();

		// Copy vectors
		y.Size(size);
		for (int index = 0, i = 0; i < x.Size(); ++i)
			for (int j = 0; j < x(i).Size(); ++j, ++index)
				y(index) = x(i)(j);
	}

	// GlmmGS
	GlmmGS::GlmmGS(Controls controls)
		: controls(controls)
	{
	}

	// Properties
	void GlmmGS::SetControls(Controls controls)
	{
		this->controls = controls;
	}

	// Methods
	void GlmmGS::Fit(const Pointer<Responses::IResponse> y, const Pointer<Offsets::IOffset> offset, const Vector<Pointer<FixedEffects::IBlock> > & x, const Vector<Pointer<RandomEffects::IBlock> > & z)
	{
		GlmmGSSolver solver(this->controls);
		solver.Fit(y, offset, x, z);

		Vector<Vector<Estimate> > beta = solver.FixedEffectsCoefficients();
		Vector<Vector<Estimate> > theta = solver.VarianceComponents();

		ToVector(this->beta, beta);
		ToVector(this->theta, theta);
	}
}
