#ifndef GLMMGSAPI_RANDOMEFFECTS_RANDOMEFFECTS_STRATIFIED_BLOCKSECTION_H
#define GLMMGSAPI_RANDOMEFFECTS_RANDOMEFFECTS_STRATIFIED_BLOCKSECTION_H

#include "../../Standard.h"
#include "../../Section.h"

namespace GlmmGSAPI
{
	namespace RandomEffects
	{
		namespace Stratified
		{
			// BlockSection
			class BlockSection : public Section
			{
			private:
				// Fields
				WeakFactor factor;
				VectorBuilder<Pointer<GlmmGS::Variables::IVariable> > variables;
				Pointer<GlmmGS::RandomEffects::Stratified::CovarianceModels::ICovarianceModel> covariance_model;
				Pointer<GlmmGS::Boosters::IBooster> booster;

				// Implementation
				void AddIntercept(int duplicate);
				void AddCovariate(Vector<const int> values, int duplicate);
				void AddCovariate(Vector<const double> values, int duplicate);
				void AddIdentityCovarianceModel(Matrix<const double> S);
				void AddPrecisionModel(Matrix<const double> R, Matrix<const double> S);
				void AddSparsePrecisionModel(const LDL::SparseMatrix<double> & R, Matrix<const double> S);
				void EndStratifiedBlock();

			public:
				// Construction
				BlockSection(const Section & section, WeakFactor factor);
			};
		}
	}
}

#endif
