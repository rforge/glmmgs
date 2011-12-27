#include "GlmmGSRAPI.h"
#include "GlmmGSAPI/GlmmGSAPI.h"
#include "GlmmGS/Controls.h"
#include "Utilities/Exceptions/Exceptions.h"
#include "GlmmGSAPI/Exceptions/Exceptions.h"

// Use 'using' only on Utilities name spaces
using namespace Utilities;
using namespace Utilities::Exceptions;

void GlmmGSRAPI_SetOutputFile(const char ** filename)
{
	const char * fname = *filename;
	if (Utilities::IO::stream != stdout)
		fclose(Utilities::IO::stream);
	Utilities::IO::stream = fopen(fname, "wt");
}

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

void GlmmGSRAPI_Begin()
{
	try
	{
		GlmmGSAPI::theApi.Begin();
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_End()
{
	try
	{
		GlmmGSAPI::theApi.End();
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

void GlmmGSRAPI_AddIdentityCovarianceModel()
{
	try
	{
		GlmmGSAPI::theApi.AddIdentityCovarianceModel();
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_AddPrecisionModel(const double * values, const int * nrows, const int * ncols)
{
	try
	{
		if (*nrows != *ncols)
			throw Utilities::Exceptions::Exception("Precision matrix must be square");
		// TODO: check matrix is symmetric
		GlmmGSAPI::theApi.AddPrecisionModel(Matrix<const double>(External<const double>(values), *nrows, *ncols));
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_AddSparsePrecisionModel(const double * values, const int * indices, const int * counts, const int * ncols)
{
	try
	{
		// Counters
		const int n = *ncols; // Number of columns
		const int nz = counts[n]; // Total number of non-zero elements

		// It is safe to use const_cast since we embed the const pointers inside constant objects
		const Vector<double> vvalues(External<double>(const_cast<double *>(values)), nz);
		const Vector<int> vindices(External<int>(const_cast<int *>(indices)), nz);
		const Vector<int> vcounts(External<int>(const_cast<int *>(counts)), n + 1);

		// Sparse matrix
		const LDL::SparseMatrix<double> R(vvalues, vindices, vcounts);

		GlmmGSAPI::theApi.AddSparsePrecisionModel(R);
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

void GlmmGSRAPI_GetFixedEffectsSize(int * size)
{
	try
	{
		*size = GlmmGSAPI::theApi.GlmmGS().FixedEffectsCoefficients().Size();
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_GetFixedEffectsEstimates(double * values, const int * size)
{
	try
	{
		const Vector<GlmmGS::Estimate> & estimates = GlmmGSAPI::theApi.GlmmGS().FixedEffectsCoefficients();
		const int n = *size;
		if (n != estimates.Size())
			throw GlmmGSAPI::Exceptions::InvalidSizeException();
		for (int i = 0; i < n; ++i)
			values[i] = estimates(i).Value();
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_GetFixedEffectsErrors(double * values, const int * size)
{
	try
	{
		const Vector<GlmmGS::Estimate> & estimates = GlmmGSAPI::theApi.GlmmGS().FixedEffectsCoefficients();
		const int n = *size;
		if (n != estimates.Size())
			throw GlmmGSAPI::Exceptions::InvalidSizeException();
		for (int i = 0; i < n; ++i)
			values[i] = sqrt(estimates(i).Variance());
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_GetRandomEffectsSize(int * size)
{
	try
	{
		*size = GlmmGSAPI::theApi.GlmmGS().RandomEffectsCoefficients().Size();
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_GetRandomEffectsEstimates(double * values, const int * size)
{
	try
	{
		const Vector<GlmmGS::Estimate> & estimates = GlmmGSAPI::theApi.GlmmGS().RandomEffectsCoefficients();
		const int n = *size;
		if (n != estimates.Size())
			throw GlmmGSAPI::Exceptions::InvalidSizeException();
		for (int i = 0; i < n; ++i)
			values[i] = estimates(i).Value();
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_GetRandomEffectsErrors(double * values, const int * size)
{
	try
	{
		const Vector<GlmmGS::Estimate> & estimates = GlmmGSAPI::theApi.GlmmGS().RandomEffectsCoefficients();
		const int n = *size;
		if (n != estimates.Size())
			throw GlmmGSAPI::Exceptions::InvalidSizeException();
		for (int i = 0; i < n; ++i)
			values[i] = sqrt(estimates(i).Variance());
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_GetCovarianceComponentsSize(int * size)
{
	try
	{
		*size = GlmmGSAPI::theApi.GlmmGS().CovarianceComponents().Size();
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_GetCovarianceComponentsEstimates(double * values, const int * size)
{
	try
	{
		const Vector<GlmmGS::Estimate> & estimates = GlmmGSAPI::theApi.GlmmGS().CovarianceComponents();
		const int n = *size;
		if (n != estimates.Size())
			throw GlmmGSAPI::Exceptions::InvalidSizeException();
		for (int i = 0; i < n; ++i)
			values[i] = estimates(i).Value();
	}
	catch (Exception & e)
	{
		GlmmGSAPI::theApi.SetLastError(e);
	}
}

void GlmmGSRAPI_GetCovarianceComponentsErrors(double * values, const int * size)
{
	try
	{
		const Vector<GlmmGS::Estimate> & estimates = GlmmGSAPI::theApi.GlmmGS().CovarianceComponents();
		const int n = *size;
		if (n != estimates.Size())
			throw GlmmGSAPI::Exceptions::InvalidSizeException();
		for (int i = 0; i < n; ++i)
			values[i] = sqrt(estimates(i).Variance());
	}
	catch (Exception & e)
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
