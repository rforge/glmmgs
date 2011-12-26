#ifndef GLMMGSAPI_GLMMGSAPI_H
#define GLMMGSAPI_GLMMGSAPI_H

#include "Standard.h"
#include "Section.h"

namespace GlmmGSAPI
{
	// GlmmGSAPI
	class GlmmGSAPI
	{
	private:
		// Fields
		static const int error_buffer_size = 1024;
		Buffer last_error;
		Stack<Pointer<Section> > sections;
		GlmmGS::GlmmGS glmmGS;

	public:
		Pointer<GlmmGS::Responses::IResponse> response;
		Pointer<GlmmGS::Offsets::IOffset> offset;
		Vector<Pointer<GlmmGS::FixedEffects::IBlock> > fixed_effects;
		Vector<Pointer<GlmmGS::RandomEffects::IBlock> > random_effects;
		bool fixed_intercept;

	public:
		// Construction
		GlmmGSAPI();
		~GlmmGSAPI();

		// Error
		void SetLastError(const Exception & e);
		void GetLastError(char * buffer, int size);

		// Sections
		void Tidy();
		void Begin();
		void End();
		void BeginResponse(WeakString<const char> family);
		void EndResponse();
		void BeginFixedEffects();
		void EndFixedEffects();
		void BeginRandomEffects();
		void EndRandomEffects();
		void BeginGlobalBlock();
		void EndGlobalBlock();
		void BeginStratifiedBlock(WeakFactor factor);
		void EndStratifiedBlock();

		// Variables
		void AddResponse(WeakVector<const int> values);
		void AddCounts(WeakVector<const int> values);
		void AddOffset(WeakVector<const int> values);
		void AddOffset(WeakVector<const double> values);
		void AddIntercept();
		void AddCovariate(WeakVector<const int> values);
		void AddCovariate(WeakVector<const double> values);

		// Covariance models
		void AddIdentityCovarianceModel();
		void AddPrecisionModel(WeakMatrix<const double> precision);
		void AddSparsePrecisionModel(const LDL::SparseMatrix<double> & precision);

		// Fit
		void Fit(GlmmGS::Controls controls);

		// Results
		const GlmmGS::GlmmGS & GlmmGS() const;
	};

	// Results
	inline
	const GlmmGS::GlmmGS & GlmmGSAPI::GlmmGS() const
	{
		return this->glmmGS;
	}

	extern GlmmGSAPI theApi;
}

#endif
