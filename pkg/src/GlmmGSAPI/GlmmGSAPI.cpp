#include "GlmmGSAPI.h"
#include "Section.h"
#include "Responses/BinomialResponseSection.h"
#include "FixedEffects/FixedEffectsSection.h"
#include "RandomEffects/RandomEffectsSection.h"
#include "Exceptions/Exceptions.h"

namespace GlmmGSAPI
{
	// The application
	GlmmGSAPI theApi;

	// GlmmGSAPI
	GlmmGSAPI::GlmmGSAPI()
		: last_error(error_buffer_size), fixed_intercept(false)
	{
	}

	GlmmGSAPI::~GlmmGSAPI()
	{
	}

	void GlmmGSAPI::SetLastError(const Exception & e)
	{
		this->last_error.Append(e.Message());
	}

	void GlmmGSAPI::GetLastError(char * buffer, int size)
	{
		if (size > 0)
		{
			int length = Min(size - 1, this->last_error.Length());
			memcpy(buffer, this->last_error.Pointer(), length);
			buffer[length] = 0;
			this->last_error.Empty();
		}
	}

	void GlmmGSAPI::Tidy()
	{
		// Reset
		this->sections.Free();
		this->last_error.Empty();
		this->response.Reset();
		this->offset.Reset();
		this->fixed_effects.Free();
		this->random_effects.Free();
		this->fixed_intercept = false;
	}

	void GlmmGSAPI::Begin()
	{
		if (this->sections.IsEmpty() == false)
			throw Exception("Invalid call: Begin");

		// Tide-up
		this->Tidy();

		// Begin
		this->sections.Push(Pointer<Section>(new(bl) Section(*this)));
	}

	void GlmmGSAPI::End()
	{
		// End
		this->sections.Pop();
		if (this->sections.IsEmpty() == false)
			throw Exception("Invalid call: End");

		// Tide-up
		this->Tidy();
	}

	void GlmmGSAPI::BeginResponse(WeakString<const char> family)
	{
		Pointer<Section> section = this->sections.Top()->BeginResponse(family);
		this->sections.Push(section);
	}

	void GlmmGSAPI::EndResponse()
	{
		this->sections.Top()->EndResponse();
		this->sections.Pop();
	}

	void GlmmGSAPI::BeginFixedEffects()
	{
		Pointer<Section> section = this->sections.Top()->BeginFixedEffects();
		this->sections.Push(section);
	}

	void GlmmGSAPI::EndFixedEffects()
	{
		this->sections.Top()->EndFixedEffects();
		this->sections.Pop();
	}

	void GlmmGSAPI::BeginRandomEffects()
	{
		Pointer<Section> section = this->sections.Top()->BeginRandomEffects();
		this->sections.Push(section);
	}

	void GlmmGSAPI::EndRandomEffects()
	{
		this->sections.Top()->EndRandomEffects();
		this->sections.Pop();
	}

	void GlmmGSAPI::BeginGlobalBlock()
	{
		Pointer<Section> section = this->sections.Top()->BeginBlock();
		this->sections.Push(section);
	}

	void GlmmGSAPI::EndGlobalBlock()
	{
		this->sections.Top()->EndBlock();
		this->sections.Pop();
	}

	void GlmmGSAPI::BeginStratifiedBlock(WeakFactor factor)
	{
		Pointer<Section> section = this->sections.Top()->BeginStratifiedBlock(factor);
		this->sections.Push(section);
	}

	void GlmmGSAPI::EndStratifiedBlock()
	{
		this->sections.Top()->EndStratifiedBlock();
		this->sections.Pop();
	}

	void GlmmGSAPI::AddResponse(Vector<const int> values)
	{
		this->sections.Top()->AddResponse(values);
	}

	void GlmmGSAPI::AddCounts(Vector<const int> values)
	{
		this->sections.Top()->AddCounts(values);
	}
	
	void GlmmGSAPI::AddOffset(Vector<const int> values)
	{
		this->sections.Top()->AddOffset(values);
	}

	void GlmmGSAPI::AddOffset(Vector<const double> values)
	{
		this->sections.Top()->AddOffset(values);
	}

	void GlmmGSAPI::AddIntercept()
	{
		this->sections.Top()->AddIntercept();
	}

	void GlmmGSAPI::AddCovariate(Vector<const int> values)
	{
		this->sections.Top()->AddCovariate(values);
	}
	
	void GlmmGSAPI::AddCovariate(Vector<const double> values)
	{
		this->sections.Top()->AddCovariate(values);
	}

	// Covariance models
	void GlmmGSAPI::AddIdentityCovarianceModel()
	{
		this->sections.Top()->AddIdentityCovarianceModel();
	}

	void GlmmGSAPI::AddPrecisionModel(Matrix<const double> precision)
	{
		this->sections.Top()->AddPrecisionModel(precision);
	}

	void GlmmGSAPI::AddSparsePrecisionModel(const LDL::SparseMatrix<double> & precision)
	{
		this->sections.Top()->AddSparsePrecisionModel(precision);
	}

	// Fit
	void GlmmGSAPI::Fit(GlmmGS::Controls controls)
	{
		// Set offset to ZeroOffset if no offset was specified
		if (this->offset.IsNull())
			this->offset.Reset(new(bl) GlmmGS::Offsets::ZeroOffset());

		// Fit the model
		this->glmmGS.Fit(this->response, this->offset, this->fixed_effects.ToVector(), this->random_effects.ToVector(), controls);
	}
}
