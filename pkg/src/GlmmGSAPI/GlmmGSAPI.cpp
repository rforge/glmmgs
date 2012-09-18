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
		: last_error(error_buffer_size)
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
		this->glmmGS.Reset();
	}

	void GlmmGSAPI::BeginModel()
	{
		if (this->sections.IsEmpty() == false)
			throw Exception("Invalid call: BeginModel");

		// Tide-up
		this->Tidy();

		// Begin
		this->sections.Push(Section::BeginModel());
	}

	void GlmmGSAPI::EndModel()
	{
		if (this->sections.Size() != 1)
			throw Exception("Invalid call: EndModel");

		// Finalize model
		this->sections.Top()->EndModel();

		// Set glmmGS object
		this->glmmGS.Reset(new(bl) GlmmGS::GlmmGS(
				this->sections.Top()->data->response,
				this->sections.Top()->data->offset,
				this->sections.Top()->data->fixed_effects.ToVector(),
				this->sections.Top()->data->random_effects.ToVector()));

		// Empty sections (and clear memory)
		this->sections.Pop();
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

	void GlmmGSAPI::AddResponse(Vector<const double> values)
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

	void GlmmGSAPI::AddIntercept(int duplicate)
	{
		this->sections.Top()->AddIntercept(duplicate);
	}

	void GlmmGSAPI::AddCovariate(Vector<const int> values, int duplicate)
	{
		this->sections.Top()->AddCovariate(values, duplicate);
	}
	
	void GlmmGSAPI::AddCovariate(Vector<const double> values, int duplicate)
	{
		this->sections.Top()->AddCovariate(values, duplicate);
	}

	// Covariance models
	void GlmmGSAPI::AddIdentityCovarianceModel(Matrix<const double> S)
	{
		this->sections.Top()->AddIdentityCovarianceModel(S);
	}

	void GlmmGSAPI::AddPrecisionModel(Matrix<const double> R, Matrix<const double> S)
	{
		this->sections.Top()->AddPrecisionModel(R, S);
	}

	void GlmmGSAPI::AddSparsePrecisionModel(const LDL::SparseMatrix<double> & R, Matrix<const double> S)
	{
		this->sections.Top()->AddSparsePrecisionModel(R, S);
	}

	// Fit
	void GlmmGSAPI::Fit(GlmmGS::Controls controls)
	{
		// Set offset to ZeroOffset if no offset was specified

		// Fit the model
		this->glmmGS->Fit(controls);
	}
}
