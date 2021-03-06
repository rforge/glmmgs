#ifndef GLMMGSAPI_RANDOMEFFECTS_RANDOMEFFECTS_GLOBAL_BLOCKSECTION_H
#define GLMMGSAPI_RANDOMEFFECTS_RANDOMEFFECTS_GLOBAL_BLOCKSECTION_H


#include "../../Standard.h"
#include "../../Section.h"

namespace GlmmGSAPI
{
	namespace RandomEffects
	{
		namespace Global
		{
			// BlockSection
			class BlockSection : public Section
			{
			private:
				// Fields
				Collections::VectorBuilder<Pointer<GlmmGS::Variables::IVariable> > variables;
				Pointer<GlmmGS::RandomEffects::Global::CovarianceModels::ICovarianceModel> covariance_model;

				// Implementation
				void AddIntercept(int duplicate);
				void AddCovariate(const ImmutableVector<int> & values, int duplicate);
				void AddCovariate(const ImmutableVector<double> & values, int duplicate);
				void AddIdentityModel(const ImmutableVector<double> & theta);
				void AddPrecisionModel(const ImmutableMatrix<double> & R, const ImmutableVector<double> & theta);
				void EndBlock();

			public:
				// Constructor
				BlockSection(const Section & section);
			};
		}
	}
}

#endif
