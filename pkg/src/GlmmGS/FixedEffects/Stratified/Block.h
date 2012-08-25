#ifndef GLMMGS_FIXEDEFFECTS_STRATIFIED_BLOCK_H
#define GLMMGS_FIXEDEFFECTS_STRATIFIED_BLOCK_H

#include "../../Standard.h"
#include "../../Variables/IVariable.h"
#include "../../Estimate.h"
#include "../../Controls.h"
#include "../IBlock.h"

namespace GlmmGS
{
	namespace FixedEffects
	{
		namespace Stratified
		{
			// Block
			class Block : public IBlock
			{
			private:
				// Fields
				Vector<Pointer<Variables::IVariable> > variables;
				WeakFactor factor;
				Vector<Vector<double> > beta;
				VectorialCholeskyDecomposition chol;

				// Implementation
				Vector<Estimate> Coefficients() const;
				int UpdateCoefficients(const Vector<double> & w, const Vector<double> & z, const Controls & controls);
				void UpdatePredictor(Vector<double> & eta) const;

			public:
				// Construction
				Block(Vector<Pointer<Variables::IVariable> > variables, WeakFactor factor);
				~Block();
			};
		}
	}
}

#endif
