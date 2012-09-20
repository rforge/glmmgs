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
		// State machine
		Stack<Pointer<Section> > sections;

		// Error variables
		static const int error_buffer_size = 1024;
		Buffer last_error;

		// Simulation variables
		Pointer<GlmmGS::GlmmGS> glmmGS;

	public:
		// Construction
		GlmmGSAPI();
		~GlmmGSAPI();

		// Error
		void SetLastError(const Exception & e);
		void GetLastError(char * buffer, int size);

		// Sections
		void Tidy();
		void BeginModel();
		void EndModel();
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
		void AddResponse(const ImmutableVector<int> & values);
		void AddResponse(const ImmutableVector<double> & values);
		void AddCounts(const ImmutableVector<int> & values);
		void AddOffset(const ImmutableVector<int> & values);
		void AddOffset(const ImmutableVector<double> & values);
		void AddIntercept(int duplicate);
		void AddCovariate(const ImmutableVector<int> & values, int duplicate);
		void AddCovariate(const ImmutableVector<double> & values, int duplicate);

		// Covariance models
		void AddIdentityCovarianceModel(Matrix<const double> S);
		void AddPrecisionModel(Matrix<const double> R, Matrix<const double> S);
		void AddSparsePrecisionModel(const LDL::SparseMatrix<double> & R, Matrix<const double> S);

		// Fit
		void Fit(GlmmGS::Controls controls);

		// Results
		const GlmmGS::GlmmGS & GlmmGS() const;
	};

	// Results
	inline
	const GlmmGS::GlmmGS & GlmmGSAPI::GlmmGS() const
	{
		return *this->glmmGS;
	}

	extern GlmmGSAPI theApi;
}

#endif
