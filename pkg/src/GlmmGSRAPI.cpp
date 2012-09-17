#include "GlmmGSRAPI.h"
#include "GlmmGSAPI/GlmmGSAPI.h"
#include "GlmmGS/Controls.h"
#include "Utilities/Exceptions/Exceptions.h"
#include "GlmmGSAPI/Exceptions/Exceptions.h"

// Use 'using' only on Utilities name spaces
using namespace Utilities;
using namespace Utilities::Exceptions;

void GlmmGSRAPI_GetLastError(char ** buffer, const int * size)
{
	if (*buffer != NULL && *size > 0)
		GlmmGSAPI::theApi.GetLastError(*buffer, *size);
}

void GlmmGSRAPI_Tidy()
{
	// No exceptions should be thrown here
	GlmmGSAPI::theApi.Tidy();
}

void GlmmGSRAPI_BeginModel()
{
	try
	{
		GlmmGSAPI::theApi.BeginModel();
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_EndModel()
{
	try
	{
		GlmmGSAPI::theApi.EndModel();
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_BeginResponse(const char ** family, const int * length)
{
	try
	{
		GlmmGSAPI::theApi.BeginResponse(WeakString<const char>(*family, *length));
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_EndResponse()
{
	try
	{
		GlmmGSAPI::theApi.EndResponse();
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_BeginFixedEffects()
{
	try
	{
		GlmmGSAPI::theApi.BeginFixedEffects();
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_EndFixedEffects()
{
	try
	{
		GlmmGSAPI::theApi.EndFixedEffects();
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_BeginRandomEffects()
{
	try
	{
		GlmmGSAPI::theApi.BeginRandomEffects();
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_EndRandomEffects()
{
	try
	{
		GlmmGSAPI::theApi.EndRandomEffects();
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_BeginGlobalBlock()
{
	try
	{
		GlmmGSAPI::theApi.BeginGlobalBlock();
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_EndGlobalBlock()
{
	try
	{
		GlmmGSAPI::theApi.EndGlobalBlock();
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_BeginStratifiedBlock(const int * levels, const int * size)
{
	try
	{
		GlmmGSAPI::theApi.BeginStratifiedBlock(WeakFactor(levels, *size));
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_EndStratifiedBlock()
{
	try
	{
		GlmmGSAPI::theApi.EndStratifiedBlock();
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_AddResponseInt(const int * values, const int * size)
{
	try
	{
		GlmmGSAPI::theApi.AddResponse(Vector<const int>(External<const int>(values), *size));
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_AddCountsInt(const int * values, const int * size)
{
	try
	{
		GlmmGSAPI::theApi.AddCounts(Vector<const int>(External<const int>(values), *size));
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

template <class TYPE>
void GlmmGSRAPI_AddOffsetImpl(const TYPE * values, const int * size)
{
	try
	{
		GlmmGSAPI::theApi.AddOffset(Vector<const TYPE>(External<const TYPE>(values), *size));
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_AddOffsetInt(const int * values, const int * size)
{
	GlmmGSRAPI_AddOffsetImpl(values, size);
}

void GlmmGSRAPI_AddOffsetDbl(const double * values, const int * size)
{
	GlmmGSRAPI_AddOffsetImpl(values, size);
}

void GlmmGSRAPI_AddIntercept()
{
	try
	{
		GlmmGSAPI::theApi.AddIntercept();
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

template <class TYPE>
void GlmmGSRAPI_AddCovariateImpl(const TYPE * values, const int * size)
{
	try
	{
		GlmmGSAPI::theApi.AddCovariate(Vector<const TYPE>(External<const TYPE>(values), *size));
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_AddCovariateInt(const int * values, const int * size)
{
	GlmmGSRAPI_AddCovariateImpl(values, size);
}

void GlmmGSRAPI_AddCovariateDbl(const double * values, const int * size)
{
	GlmmGSRAPI_AddCovariateImpl(values, size);
}

template <class TYPE>
void GlmmGSRAPI_AddCovariatesImpl(const TYPE * values, const int * dimensions)
{
	try
	{
		const int nrows = dimensions[0];
		const int ncols = dimensions[1];
		for (int j = 0; j < ncols; ++j)
			GlmmGSAPI::theApi.AddCovariate(Vector<const TYPE>(External<const TYPE>(values + j * nrows), nrows));
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_AddCovariatesInt(const int * values, const int * dimensions)
{
	GlmmGSRAPI_AddCovariatesImpl(values, dimensions);
}

void GlmmGSRAPI_AddCovariatesDbl(const double * values, const int * dimensions)
{
	GlmmGSRAPI_AddCovariatesImpl(values, dimensions);
}

void GlmmGSRAPI_AddIdentityCovarianceModel(
		const double * S, const int * nrowsS, const int * ncolsS)
{
	try
	{
		if (*nrowsS != *ncolsS)
			throw Utilities::Exceptions::Exception("Covariance components matrix must be square");

		// TODO: check matrix is symmetric

		GlmmGSAPI::theApi.AddIdentityCovarianceModel(
			Matrix<const double>(External<const double>(S), *nrowsS, *ncolsS));
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_AddPrecisionModel(
		const double * R, const int * nrowsR, const int * ncolsR,
		const double * S, const int * nrowsS, const int * ncolsS)
{
	try
	{
		if (*nrowsR != *ncolsR)
			throw Utilities::Exceptions::Exception("Precision matrix must be square");

		if (*nrowsS != *ncolsS)
			throw Utilities::Exceptions::Exception("Covariance components matrix must be square");

		// TODO: check matrices are symmetric

		GlmmGSAPI::theApi.AddPrecisionModel(
				Matrix<const double>(External<const double>(R), *nrowsR, *ncolsR),
				Matrix<const double>(External<const double>(S), *nrowsS, *ncolsS));
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_AddSparsePrecisionModel(
		const double * values, const int * indices, const int * counts, const int * ncols,
		const double * S, const int * nrowsS, const int * ncolsS)
{
	try
	{
		if (*nrowsS != *ncolsS)
			throw Utilities::Exceptions::Exception("Covariance components matrix must be square");

		// TODO: check matrices are symmetric

		// Counters
		const int n = *ncols; // Number of columns
		const int nz = counts[n]; // Total number of non-zero elements

		// Sparse matrix
		const LDL::SparseMatrix<double> R(
				Vector<double>(External<double>(const_cast<double *>(values)), nz),
				Vector<int>(External<int>(const_cast<int *>(indices)), nz),
				Vector<int>(External<int>(const_cast<int *>(counts)), n + 1));

		GlmmGSAPI::theApi.AddSparsePrecisionModel(R,
				Matrix<const double>(External<const double>(S), *nrowsS, *ncolsS));
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_Fit(const double * relative_tolerance, const double * absolute_tolerance,
		const int * maxiter, const int * verbose)
{
	try
	{
		GlmmGS::Controls controls(*relative_tolerance, *absolute_tolerance, *maxiter, (*verbose) != 0);
		GlmmGSAPI::theApi.Fit(controls);
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

// Results - Dense fixed effects
void GlmmGSRAPI_GetFixefDenseBlock(
		const int * index,
		double * estm,
		double * vcov,
		const int * nvars)
{
	try
	{
		// Get fixed effects
		typedef GlmmGS::FixedEffects::IBlock I;
		Vector<Pointer<I> > fixef = GlmmGSAPI::theApi.GlmmGS().FixedEffects();
		if (*index < 0 || *index >= fixef.Size())
			throw Exception("Invalid block index");
		typedef GlmmGS::FixedEffects::Global::Block T;
		const T & block = dynamic_cast<const T &>(*fixef(*index));

		// Get estimates
		Copy(estm, *nvars, block.Coefficients());
		Copy(vcov, Math::Square(*nvars), block.Covariance());
	}
	catch  (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

// Results - Stratified fixed effects
void GlmmGSRAPI_GetFixefStratifiedBlock(
		const int * index,
		double * estm,
		double * vcov,
		const int * nvars,
		const int * nlevels)
{
	try
	{
		// Get fixed effects
		typedef GlmmGS::FixedEffects::IBlock I;
		Vector<Pointer<I> > fixef = GlmmGSAPI::theApi.GlmmGS().FixedEffects();
		if (*index < 0 || *index >= fixef.Size())
			throw Exception("Invalid block index");
		typedef GlmmGS::FixedEffects::Stratified::Block T;
		const T & block = dynamic_cast<const T &>(*fixef(*index));

		// Get estimates
		Copy(estm, (*nvars) * (*nlevels), block.Coefficients());
		Copy(vcov, Math::Square(*nvars) * (*nlevels), block.Covariance());
	}
	catch  (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

// Results - Random effects
void GlmmGSRAPI_GetRanefDenseBlock(
		const int * index,
		double * estm,
		const int * nvars)
{
	try
	{
		// Get random effects
		typedef GlmmGS::RandomEffects::IBlock I;
		Vector<Pointer<I> > ranef = GlmmGSAPI::theApi.GlmmGS().RandomEffects();
		if (*index < 0 || *index >= ranef.Size())
			throw Exception("Invalid block index");
		typedef GlmmGS::RandomEffects::Global::Block T;
		const T & block = dynamic_cast<const T &>(*ranef(*index));

		// Get estimates
		Copy(estm, *nvars, block.Coefficients());
	}
	catch  (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_GetVCompDenseBlock(
		const int * index,
		double * estm,
		double * vcov,
		const int * size)
{
	try
	{
		// Get random effects
		typedef GlmmGS::RandomEffects::IBlock I;
		Vector<Pointer<I> > ranef = GlmmGSAPI::theApi.GlmmGS().RandomEffects();
		if (*index < 0 || *index >= ranef.Size())
			throw Exception("Invalid block index");
		typedef GlmmGS::RandomEffects::Global::Block T;
		const T & block = dynamic_cast<const T &>(*ranef(*index));

		// Get estimates
		Copy(estm, (*size), block.CovarianceModel()->Components());
		Copy(vcov, Math::Square(*size), block.CovarianceModel()->Covariance());
	}
	catch  (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_GetRanefStratifiedBlock(
		const int * index,
		double * estm,
		const int * nvars,
		const int * nlevels)
{
	try
	{
		// Get random effects
		typedef GlmmGS::RandomEffects::IBlock I;
		Vector<Pointer<I> > ranef = GlmmGSAPI::theApi.GlmmGS().RandomEffects();
		if (*index < 0 || *index >= ranef.Size())
			throw Exception("Invalid block index");
		typedef GlmmGS::RandomEffects::Stratified::Block T;
		const T & block = dynamic_cast<const T &>(*ranef(*index));

		// Get estimates
		Copy(estm, (*nvars) * (*nlevels), block.Coefficients());
	}
	catch  (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_GetVCompStratifiedBlock(
		const int * index,
		double * estm,
		double * vcov,
		const int * size)
{
	try
	{
		// Get random effects
		typedef GlmmGS::RandomEffects::IBlock I;
		Vector<Pointer<I> > ranef = GlmmGSAPI::theApi.GlmmGS().RandomEffects();
		if (*index < 0 || *index >= ranef.Size())
			throw Exception("Invalid block index");
		typedef GlmmGS::RandomEffects::Stratified::Block T;
		const T & block = dynamic_cast<const T &>(*ranef(*index));

		// Get estimates
		Copy(estm, (*size), block.CovarianceModel()->Components());
		Copy(vcov, Math::Square(*size), block.CovarianceModel()->Covariance());
	}
	catch  (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_GetIterations(int * iterations)
{
	try
	{
		*iterations = GlmmGSAPI::theApi.GlmmGS().Iterations();
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}
