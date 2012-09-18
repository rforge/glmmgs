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
		void AddResponse(Vector<const int> values);
		void AddResponse(Vector<const double> values);
		void AddCounts(Vector<const int> values);
		void AddOffset(Vector<const int> values);
		void AddOffset(Vector<const double> values);
		void AddIntercept(int duplicate);
		void AddCovariate(Vector<const int> values, int duplicate);
		void AddCovariate(Vector<const double> values, int duplicate);

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
