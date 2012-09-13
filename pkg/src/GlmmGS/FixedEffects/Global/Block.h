#ifndef GLMMGS_FIXEDEFFECTS_GLOBAL_BLOCK_H
#define GLMMGS_FIXEDEFFECTS_GLOBAL_BLOCK_H

#include "../../Standard.h"
#include "../../Variables/IVariable.h"
#include "../../Estimate.h"
#include "../../Controls.h"
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
				int UpdateCoefficients(const Vector<double> & w, const Vector<double> & z, const Controls & controls);
				void UpdatePredictor(Vector<double> & eta) const;

			public:
				// Construction
				Block(Vector<Pointer<Variables::IVariable> > variables);
				~Block();

				// Properties
				Vector<double> Coefficients() const;
				TriangularMatrix<double> Covariance() const;
			};
		}
	}
}

#endif
