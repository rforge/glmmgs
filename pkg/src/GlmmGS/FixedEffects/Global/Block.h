#ifndef GLMMGS_FIXEDEFFECTS_GLOBAL_BLOCK_H
#define GLMMGS_FIXEDEFFECTS_GLOBAL_BLOCK_H

#include "../../Standard.h"
#include "../../Variables/IVariable.h"
#include "../../Control.h"
#include "../IBlock.h"

namespace GlmmGS
{
	namespace FixedEffects
	{
		namespace Global
		{
			// Block
			class Block : public IBlock
			{
			private:
				// Fields
				Vector<Pointer<Variables::IVariable> > variables;
				Vector<double> beta;
				CholeskyDecomposition chol;

				// Implementation
				int UpdateCoefficients(const ImmutableVector<double> & weight, const ImmutableVector<double> & values, const Control & control);
				void UpdatePredictor(Vector<double> & eta) const;

			public:
				// Construction
				Block(Vector<Pointer<Variables::IVariable> > variables);
				~Block();

				// Properties
				const ImmutableVector<double> & Coefficients() const;
				TriangularMatrix<double> Covariance() const;
			};
		}
	}
}

#endif
