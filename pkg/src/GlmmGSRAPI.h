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
	void GlmmGSRAPI_AddCountsInt(const int * values, const int * size);
	void GlmmGSRAPI_AddOffsetInt(const int * values, const int * size);
	void GlmmGSRAPI_AddOffsetDbl(const double * values, const int * size);
	void GlmmGSRAPI_AddIntercept();
	void GlmmGSRAPI_AddCovariateInt(const int * values, const int * size);
	void GlmmGSRAPI_AddCovariateDbl(const double * values, const int * size);
	void GlmmGSRAPI_AddCovariatesInt(const int * values, const int * dim);
	void GlmmGSRAPI_AddCovariatesDbl(const double * values, const int * dimensions);

	// Covariance models
	void GlmmGSRAPI_AddIdentityCovarianceModel();
	void GlmmGSRAPI_AddPrecisionModel(const double * values, const int * nrows, const int * ncols);
	void GlmmGSRAPI_AddSparsePrecisionModel(const double * values, const int * indices, const int * counts, const int * ncols);

	// Fit
	void GlmmGSRAPI_Fit(const double * relative_tolerance, const double * absolute_tolerance,
			const int * maxiter, const int * verbose);

	// Results - Fixed effects
	void GlmmGSRAPI_GetFixefDenseBlock(
			const int * block,
			const double * beta,
			const double * beta_cov,
			const int * nvars);

	void GlmmGSRAPI_GetFixefStratifiedBlock(
			const int * block,
			const double * beta,
			const double * beta_cov,
			const int * nvars,
			const int * nlevels);

	// Results - Random effects
	void GlmmGSRAPI_GetRanefDenselBlockDense(
			const int * block,
			const double * beta,
			const double * beta_cov,
			const double * theta,
			const double * theta_cov,
			const int * nvars);

	void GlmmGSRAPI_GetRanefStratifiedBlockDense(
			const int * block,
			const double * beta,
			const double * beta_cov,
			const double * theta,
			const double * theta_cov,
			const int * nvars,
			const int * nlevels);

	void GlmmGSRAPI_GetRanefStratifiedBlockStratified(
			const int * block,
			const double * beta,
			const double * beta_cov,
			const double * theta,
			const double * theta_cov,
			const int * nvars,
			const int * nlevels);

	void GlmmGSRAPI_GetRanefStratifiedBlockSparse(
			const int * block,
			const double * beta,
			const double * beta_cov,
			const double * theta,
			const double * theta_cov_values, const int * theta_cov_indices, const int * theta_cov_counts, const int * theta_cov_ncols,
			const int * nvars,
			const int * nlevels);

	void GlmmGSRAPI_GetIterations(int * iterations);
}

#endif
