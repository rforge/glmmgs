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
		Vector<GlmmGS::Estimate> beta; // Fixed-effects coefficients estimates
		Vector<GlmmGS::Estimate> theta; // Variance component estimates

	public:
		Pointer<GlmmGS::Responses::IResponse> response;
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
		void Begin();
		void End();
		void ForceEnd();
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
		void AddIntercept();
		void AddCovariate(WeakVector<const int> values);
		void AddCovariate(WeakVector<const double> values);

		// Covariance models
		void AddIdentityCovarianceModel();
		void AddPrecisionModel(WeakMatrix<const double> precision);

		// Fit
		void Fit(GlmmGS::Controls controls);

		// Results
		int GetFixedEffectsSize() const;
		void GetFixedEffectsEstimates(WeakVector<double> values) const;
		void GetFixedEffectsErrors(WeakVector<double> values) const;
		int GetVarianceComponentsSize() const;
		void GetVarianceComponentsEstimates(WeakVector<double> values) const;
		void GetVarianceComponentsErrors(WeakVector<double> values) const;
	};

	extern GlmmGSAPI the_api;
}
