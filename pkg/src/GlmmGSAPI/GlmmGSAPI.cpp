#include "GlmmGSAPI.h"
#include "Section.h"
#include "Responses/BinomialResponseSection.h"
#include "FixedEffects/FixedEffectsSection.h"
#include "RandomEffects/RandomEffectsSection.h"
#include "Exceptions/Exceptions.h"
#include "../GlmmGS/Estimate.h"

namespace GlmmGSAPI
{
	// The application
	GlmmGSAPI the_api;

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
			buffer[length]= 0;
			this->last_error.Empty();
		}
	}

	void GlmmGSAPI::Begin()
	{
		if (this->sections.IsEmpty() == false)
			throw Exception("Invalid call: Begin");
		this->sections.Push(Pointer<Section>(new(bl) Section(*this)));
	}

	void GlmmGSAPI::End()
	{
		this->sections.Pop();
		if (this->sections.IsEmpty() == false)
			throw Exception("Invalid call: End");

		// Reset
		this->last_error.Empty();
		this->response.Reset();
		this->fixed_effects.Free();
		this->random_effects.Free();
	}

	void GlmmGSAPI::ForceEnd()
	{
		// Reset
		this->sections.Free();
		this->last_error.Empty();
		this->response.Reset();
		this->fixed_effects.Free();
		this->random_effects.Free();
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

	void GlmmGSAPI::AddResponse(WeakVector<const int> values)
	{
		this->sections.Top()->AddResponse(values);
	}

	void GlmmGSAPI::AddCounts(WeakVector<const int> values)
	{
		this->sections.Top()->AddCounts(values);
	}
	
	void GlmmGSAPI::AddIntercept()
	{
		this->sections.Top()->AddIntercept();
	}

	void GlmmGSAPI::AddCovariate(WeakVector<const int> values)
	{
		this->sections.Top()->AddCovariate(values);
	}
	
	void GlmmGSAPI::AddCovariate(WeakVector<const double> values)
	{
		this->sections.Top()->AddCovariate(values);
	}

	// Covariance models
	void GlmmGSAPI::AddIdentityCovarianceModel()
	{
		this->sections.Top()->AddIdentityCovarianceModel();
	}

	void GlmmGSAPI::AddPrecisionModel(WeakMatrix<const double> precision)
	{
		this->sections.Top()->AddPrecisionModel(precision);
	}

	// Fit
	void GlmmGSAPI::Fit(GlmmGS::Controls controls)
	{
		GlmmGS::GlmmGS glmmGS(controls);
		glmmGS.Fit(this->response, this->fixed_effects, this->random_effects);
		this->beta = glmmGS.FixedEffectsCoefficients();
		this->theta = glmmGS.VarianceComponents();
	}

	// Results
	int GlmmGSAPI::GetFixedEffectsSize() const
	{
		return this->beta.Size();
	}

	void GlmmGSAPI::GetFixedEffectsEstimates(WeakVector<double> values) const
	{
		if (values.Size() != this->beta.Size())
			throw Exceptions::InvalidSizeException();
		for (int i = 0; i < this->beta.Size(); ++i)
			values(i) = this->beta(i).Value();
	}

	void GlmmGSAPI::GetFixedEffectsErrors(WeakVector<double> values) const
	{
		if (values.Size() != this->beta.Size())
			throw Exceptions::InvalidSizeException();
		for (int i = 0; i < this->beta.Size(); ++i)
			values(i) = sqrt(this->beta(i).Variance());
	}

	int GlmmGSAPI::GetVarianceComponentsSize() const
	{
		return this->theta.Size();
	}

	void GlmmGSAPI::GetVarianceComponentsEstimates(WeakVector<double> values) const
	{
		if (values.Size() != this->theta.Size())
			throw Exceptions::InvalidSizeException();
		for (int i = 0; i < this->theta.Size(); ++i)
			values(i) = this->theta(i).Value();
	}

	void GlmmGSAPI::GetVarianceComponentsErrors(WeakVector<double> values) const
	{
		if (values.Size() != this->theta.Size())
			throw Exceptions::InvalidSizeException();
		for (int i = 0; i < this->theta.Size(); ++i)
			values(i) = sqrt(this->theta(i).Variance());
	}
}
