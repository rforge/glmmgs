#pragma once

#if defined GLMMGS_WIN
	#if defined GLMMGS_EXPORT
		#define GLMMGSAPI __declspec(dllexport)
	#elif defined GLMMGS_IMPORT
		#define GLMMGSAPI __declspec(dllimport)
	#else
		#define GLMMGSAPI
	#endif
#else
  #if __GNUC__ >= 4
    #define GLMMGSAPI __attribute__ ((visibility("default")))
  #else
    #define GLMMGSAPI
  #endif
#endif

extern "C"
{
	// Error
	GLMMGSAPI void GlmmGSRAPI_GetLastError(char ** buffer, const int * size);

	// Sections
	GLMMGSAPI void GlmmGSRAPI_Begin();
	GLMMGSAPI void GlmmGSRAPI_End();
	GLMMGSAPI void GlmmGSRAPI_ForceEnd();
	GLMMGSAPI void GlmmGSRAPI_BeginResponse(const char ** family, const int * length);
	GLMMGSAPI void GlmmGSRAPI_EndResponse();
	GLMMGSAPI void GlmmGSRAPI_BeginFixedEffects();
	GLMMGSAPI void GlmmGSRAPI_EndFixedEffects();
	GLMMGSAPI void GlmmGSRAPI_BeginRandomEffects();
	GLMMGSAPI void GlmmGSRAPI_EndRandomEffects();
	GLMMGSAPI void GlmmGSRAPI_BeginGlobalBlock();
	GLMMGSAPI void GlmmGSRAPI_EndGlobalBlock();
	GLMMGSAPI void GlmmGSRAPI_BeginStratifiedBlock(const int * levels, const int * size);
	GLMMGSAPI void GlmmGSRAPI_EndStratifiedBlock();

	// Variables
	GLMMGSAPI void GlmmGSRAPI_AddResponseInt(const int * values, const int * size);
	GLMMGSAPI void GlmmGSRAPI_AddCountsInt(const int * values, const int * size);
	GLMMGSAPI void GlmmGSRAPI_AddIntercept();
	GLMMGSAPI void GlmmGSRAPI_AddCovariateInt(const int * values, const int * size);
	GLMMGSAPI void GlmmGSRAPI_AddCovariateDbl(const double * values, const int * size);
	GLMMGSAPI void GlmmGSRAPI_AddCovariatesInt(const int * values, const int * dim);
	GLMMGSAPI void GlmmGSRAPI_AddCovariatesDbl(const double * values, const int * dimensions);

	// Covariance models
	GLMMGSAPI void GlmmGSRAPI_AddIdentityCovarianceModel();
	GLMMGSAPI void GlmmGSRAPI_AddPrecisionModel(const double * values, const int * nrows, const int * ncols);

	// Fit
    GLMMGSAPI void GlmmGSRAPI_Fit(const double * relative_tolerance, const double * absolute_tolerance, const int * maxiter);

	// Results
    GLMMGSAPI void GlmmGSRAPI_GetFixedEffectsSize(int * size);
    GLMMGSAPI void GlmmGSRAPI_GetFixedEffectsEstimates(double * values, const int * size);
    GLMMGSAPI void GlmmGSRAPI_GetFixedEffectsErrors(double * values, const int * size);
    GLMMGSAPI void GlmmGSRAPI_GetVarianceComponentsSize(int * size);
    GLMMGSAPI void GlmmGSRAPI_GetVarianceComponentsEstimates(double * values, const int * size);
    GLMMGSAPI void GlmmGSRAPI_GetVarianceComponentsErrors(double * values, const int * size);
}
