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
				Collections::VectorBuilder<Pointer<GlmmGS::Variables::IVariable> > variables;
				Pointer<GlmmGS::RandomEffects::Stratified::CovarianceModels::ICovarianceModel> covariance_model;

				// Implementation
				void AddIntercept(int duplicate);
				void AddCovariate(const ImmutableVector<int> & values, int duplicate);
				void AddCovariate(const ImmutableVector<double> & values, int duplicate);
				void AddIdentityModel(const ImmutableVector<double> & theta);
				void AddMultivariateIdentityModel(const ImmutableVector<double> & theta);
				void AddPrecisionModel(const ImmutableMatrix<double> & R, const ImmutableVector<double> & theta);
				void AddSparsePrecisionModel(const LDL::SparseMatrix<double> & R, const ImmutableVector<double> & theta);
				void EndStratifiedBlock();

			public:
				// Construction
				BlockSection(const Section & section, WeakFactor factor);
			};
		}
	}
}

#endif
