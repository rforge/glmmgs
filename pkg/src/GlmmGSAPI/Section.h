#ifndef GLMMGSAPI_SECTION_H
#define GLMMGSAPI_SECTION_H

#include "Standard.h"

namespace GlmmGSAPI
{
	// Forward declaration
	class GlmmGSAPI;

	// Section
	class Section
	{
	// Disable assignment operator
	const Section & operator =(const Section &);

	protected:
		// Fields
		GlmmGSAPI & api;

	public:
		// Construction
		Section(GlmmGSAPI & api);
		virtual ~Section();
		
		// Methods
		virtual Pointer<Section> BeginResponse(WeakString<const char> family);
		virtual void EndResponse();
		virtual Pointer<Section> BeginFixedEffects();
		virtual void EndFixedEffects();
		virtual Pointer<Section> BeginRandomEffects();
		virtual void EndRandomEffects();
		virtual Pointer<Section> BeginBlock();
		virtual void EndBlock();
		virtual Pointer<Section> BeginStratifiedBlock(WeakFactor factor);
		virtual void EndStratifiedBlock();
		virtual void AddResponse(Vector<const int> values);
		virtual void AddCounts(Vector<const int> values);
		virtual void AddOffset(Vector<const int> values);
		virtual void AddOffset(Vector<const double> values);
		virtual void AddIntercept();
		virtual void AddCovariate(Vector<const int> values);
		virtual void AddCovariate(Vector<const double> values);
		virtual void AddIdentityCovarianceModel();
		virtual void AddPrecisionModel(Matrix<const double> precision);
		virtual void AddSparsePrecisionModel(const LDL::SparseMatrix<double> & precision);
	};
}

#endif
