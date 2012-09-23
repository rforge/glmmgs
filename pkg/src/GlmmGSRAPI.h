#ifndef GLMMGSRAPI_H
#define GLMMGSRAPI_H

extern "C"
{
	// Error
	void GlmmGSRAPI_GetLastError(char ** buffer, const int * size);

	// Sections
	void GlmmGSRAPI_Tidy();
	void GlmmGSRAPI_BeginModel();
	void GlmmGSRAPI_EndModel();
	void GlmmGSRAPI_BeginResponse(const char ** family, const int * length);
	void GlmmGSRAPI_EndResponse();
	void GlmmGSRAPI_BeginFixedEffects();
	void GlmmGSRAPI_EndFixedEffects();
	void GlmmGSRAPI_BeginRandomEffects();
	void GlmmGSRAPI_EndRandomEffects();
	void GlmmGSRAPI_BeginGlobalBlock();
	void GlmmGSRAPI_EndGlobalBlock();
	void GlmmGSRAPI_BeginStratifiedBlock(const int * levels, const int * size);
	void GlmmGSRAPI_EndStratifiedBlock();

	// Variables
	void GlmmGSRAPI_AddResponseInt(const int * values, const int * size);
	void GlmmGSRAPI_AddResponseDbl(const double * values, const int * size);
	void GlmmGSRAPI_AddCountsInt(const int * values, const int * size);
	void GlmmGSRAPI_AddOffsetInt(const int * values, const int * size);
	void GlmmGSRAPI_AddOffsetDbl(const double * values, const int * size);
	void GlmmGSRAPI_AddIntercept(const int * duplicate);
	void GlmmGSRAPI_AddCovariateInt(const int * values, const int * size, const int * duplicate);
	void GlmmGSRAPI_AddCovariateDbl(const double * values, const int * size, const int * duplicate);
	void GlmmGSRAPI_AddCovariatesInt(const int * values, const int * dim, const int * duplicate);
	void GlmmGSRAPI_AddCovariatesDbl(const double * values, const int * dim, const int * duplicate);

	// Covariance models
	void GlmmGSRAPI_AddIdentityCovarianceModel(
			const double * S, const int * dimS);

	void GlmmGSRAPI_AddPrecisionModel(
			const double * R, const int * dimR,
			const double * S, const int * dimS);

	void GlmmGSRAPI_AddSparsePrecisionModel(
			const double * values, const int * indices, const int * counts, const int * ncols,
			const double * S, const int * dimS);

	// Fit
	void GlmmGSRAPI_Fit(
			const double * relative_tolerance,
			const double * absolute_tolerance,
			const int * maxiter,
			const int * verbose,
			const double * max_updates,
			const double * max_values);

	// Results - Fixed effects
	void GlmmGSRAPI_GetFixefDenseBlock(
			const int * index,
			double * coef,
			double * vcov,
			const int * nvars);

	void GlmmGSRAPI_GetFixefStratifiedBlock(
			const int * index,
			double * estm,
			double * vcov,
			const int * nvars,
			const int * nlevels);

	// Results - Random effects
	void GlmmGSRAPI_GetRanefDenseBlock(
			const int * index,
			double * estm,
			const int * nvars);

	void GlmmGSRAPI_GetVCompDenseBlock(
			const int * index,
			double * estm,
			double * vcov,
			const int * size);

	void GlmmGSRAPI_GetRanefStratifiedBlock(
			const int * index,
			double * estm,
			const int * nvars,
			const int * nlevels);

	void GlmmGSRAPI_GetVCompStratifiedBlock(
			const int * index,
			double * estm,
			double * vcov,
			const int * size);

	void GlmmGSRAPI_GetIterations(int * iterations, const int * size);
}

#endif
