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
				VectorBuilder<Pointer<GlmmGS::Variables::IVariable> > variables;
				Pointer<GlmmGS::RandomEffects::Global::CovarianceModels::ICovarianceModel> covariance_model;

				// Implementation
				void AddIntercept();
				void AddCovariate(Vector<const int> values);
				void AddCovariate(Vector<const double> values);
				void AddIdentityCovarianceModel();
				void AddPrecisionModel(Matrix<const double> precision);
				void EndBlock();

			public:
				// Constructor
				BlockSection(const Section & section);
			};
		}
	}
}

#endif
