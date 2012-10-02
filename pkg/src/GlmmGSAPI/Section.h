#ifndef GLMMGSAPI_SECTION_H
#define GLMMGSAPI_SECTION_H

#include "Standard.h"

namespace GlmmGSAPI
{
	// Forward declaration
	class GlmmGSAPI;

	// Section
	class Section
	{
	private:
		// Data structure
		struct Data
		{
			// Fields
			Pointer<GlmmGS::Responses::IResponse> response;
			Pointer<GlmmGS::Offsets::IOffset> offset;
			VectorBuilder<Pointer<GlmmGS::FixedEffects::IBlock> > fixed_effects;
			VectorBuilder<Pointer<GlmmGS::RandomEffects::IBlock> > random_effects;
		};

	public:
		// Data
		Pointer<Data> data;

		// Construction
	private:
		Section();
	public:
		virtual ~Section();
		
		// Section methods
		static Pointer<Section> BeginModel();
		void EndModel();
		Pointer<Section> BeginResponse(WeakString<const char> family);
		Pointer<Section> BeginFixedEffects();
		virtual void EndFixedEffects();
		Pointer<Section> BeginRandomEffects();
		virtual void EndRandomEffects();

		// Response section methods
		virtual void AddResponse(const ImmutableVector<int> & values);
		virtual void AddResponse(const ImmutableVector<double> & values);
		virtual void AddCounts(const ImmutableVector<int> & values);
		virtual void EndResponse();

		// Offset
		void AddOffset(const ImmutableVector<int> & values);
		void AddOffset(const ImmutableVector<double> & values);

		// Blocks
		virtual Pointer<Section> BeginBlock();
		virtual void EndBlock();
		virtual Pointer<Section> BeginStratifiedBlock(WeakFactor factor);
		virtual void EndStratifiedBlock();

		// Covariates
		virtual void AddIntercept(int duplicate);
		virtual void AddCovariate(const ImmutableVector<int> & values, int duplicate);
		virtual void AddCovariate(const ImmutableVector<double> & values, int duplicate);

		// Covariance models
		virtual void AddIdentityModel(const ImmutableVector<double> & theta);
		virtual void AddMultivariateIdentityModel(const ImmutableVector<double> & theta);
		virtual void AddPrecisionModel(const ImmutableMatrix<double> & R, const ImmutableVector<double> & theta);
		virtual void AddSparsePrecisionModel(const LDL::SparseMatrix<double> & R, const ImmutableVector<double> & theta);
	};
}

#endif
