#include "Standard.h"
#include "Section.h"
#include "GlmmGSAPI.h"
#include "Responses/BinomialResponseSection.h"
#include "Responses/PoissonResponseSection.h"
#include "FixedEffects/FixedEffectsSection.h"
#include "RandomEffects/RandomEffectsSection.h"

namespace GlmmGSAPI
{
	// Section
	Section::Section()
		: data(new(bl) Data)
	{
	}

	Section::~Section()
	{

	}

	Pointer<Section> Section::BeginModel()
	{
		return Pointer<Section>(new(bl) Section);
	}

	void Section::EndModel()
	{
		// Finalize model

		// If offset is NULL set it to default
		if (this->data->offset.IsNull())
			this->data->offset.Reset(new(bl) GlmmGS::Offsets::ZeroOffset);
	}

	Pointer<Section> Section::BeginResponse(WeakString<const char> family)
	{
		if (family == "binomial")
		{
			typedef Responses::BinomialResponseSection T;
			return Pointer<T>(new(bl) T(*this));
		}
		else if (family == "poisson")
		{
			typedef Responses::PoissonResponseSection T;
			return Pointer<T>(new(bl) T(*this));
		}
		else
			throw Exception("Non supported response family");
	}

	void Section::EndResponse()
	{
		throw Exception("Invalid call: EndResponse");
	}

	void Section::AddOffset(Vector<const int> values)
	{
		if (this->data->offset.IsNull() == false)
			throw Exception("Multiple offsets");
		typedef GlmmGS::Offsets::VectorOffset<const int>  T;
		this->data->offset.Reset(new(bl) T(values));
	}

	void Section::AddOffset(Vector<const double> values)
	{
		if (this->data->offset.IsNull() == false)
			throw Exception("Multiple offsets");
		typedef GlmmGS::Offsets::VectorOffset<const double>  T;
		this->data->offset.Reset(new(bl) T(values));
	}

	Pointer<Section> Section::BeginFixedEffects()
	{
		typedef FixedEffects::FixedEffectsSection T;
		return Pointer<T>(new(bl) T(*this));
	}

	void Section::EndFixedEffects()
	{
		throw Exception("Invalid call: EndFixedEffects");
	}

	Pointer<Section> Section::BeginRandomEffects()
	{
		typedef RandomEffects::RandomEffectsSection T;
		return Pointer<T>(new(bl) T(*this));
	}

	void Section::EndRandomEffects()
	{
		throw Exception("Invalid call: EndRandomEffects");
	}

	Pointer<Section> Section::BeginBlock()
	{
		throw Exception("Invalid call: BeginBlock");
	}

	void Section::EndBlock()
	{
		throw Exception("Invalid call: EndBlock");
	}

	Pointer<Section> Section::BeginStratifiedBlock(WeakFactor)
	{
		throw Exception("Invalid call: BeginStratifiedBlock");
	}

	void Section::EndStratifiedBlock()
	{
		throw Exception("Invalid call: EndStratifiedBlock");
	}

	void Section::AddResponse(Vector<const int>)
	{
		throw Exception("Invalid call: AddResponse");
	}

	void Section::AddCounts(Vector<const int>)
	{
		throw Exception("Invalid call: AddCounts");
	}

	void Section::AddIntercept(int)
	{
		throw Exception("Invalid call: AddIntercept");
	}

	void Section::AddCovariate(Vector<const int>, int)
	{
		throw Exception("Invalid call: AddCovariate");
	}

	void Section::AddCovariate(Vector<const double>, int)
	{
		throw Exception("Invalid call: AddCovariate");
	}

	void Section::AddIdentityCovarianceModel(Matrix<const double>)
	{
		throw Exception("Invalid call: AddIdentityCovarianceModel");
	}

	void Section::AddPrecisionModel(Matrix<const double>, Matrix<const double>)
	{
		throw Exception("Invalid call: AddPrecisionModel");
	}

	void Section::AddSparsePrecisionModel(const LDL::SparseMatrix<double> &, Matrix<const double>)
	{
		throw Exception("Invalid call: AddSparsePrecisionModel");
	}
}
