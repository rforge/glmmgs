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
			bool fixed_intercept;
			Pointer<GlmmGS::Responses::IResponse> response;
			Pointer<GlmmGS::Offsets::IOffset> offset;
			VectorBuilder<Pointer<GlmmGS::FixedEffects::IBlock> > fixed_effects;
			VectorBuilder<Pointer<GlmmGS::RandomEffects::IBlock> > random_effects;

			// Constructor
			Data() : fixed_intercept(false) {}
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
		void AddOffset(Vector<const int> values);
		void AddOffset(Vector<const double> values);
		Pointer<Section> BeginFixedEffects();
		Pointer<Section> BeginRandomEffects();

		// Response section methods
		virtual void AddResponse(Vector<const int> values);
		virtual void AddCounts(Vector<const int> values);
		virtual void EndResponse();

		// Fixed and random effect common methods
		virtual Pointer<Section> BeginBlock();
		virtual void EndBlock();
		virtual Pointer<Section> BeginStratifiedBlock(WeakFactor factor);
		virtual void EndStratifiedBlock();
		virtual void AddIntercept(int duplicate);
		virtual void AddCovariate(Vector<const int> values, int duplicate);
		virtual void AddCovariate(Vector<const double> values, int duplicate);

		// Fixed effect methods
		virtual void EndFixedEffects();

		// Random effect methods
		virtual void AddIdentityCovarianceModel(Matrix<const double> S);
		virtual void AddPrecisionModel(Matrix<const double> R, Matrix<const double> S);
		virtual void AddSparsePrecisionModel(const LDL::SparseMatrix<double> & R, Matrix<const double> S);
		virtual void EndRandomEffects();
	};
}

#endif
