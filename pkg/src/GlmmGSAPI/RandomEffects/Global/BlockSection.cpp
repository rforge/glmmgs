#include "../../Standard.h"
#include "../../GlmmGSAPI.h"
#include "../../Exceptions/Exceptions.h"
#include "BlockSection.h"

namespace GlmmGSAPI
{
	namespace RandomEffects
	{
		namespace Global
		{
			// Construction
			BlockSection::BlockSection(const Section & section)
				: Section(section)
			{

			}

			// Implementation
			void BlockSection::AddIntercept(int duplicate)
			{
				if (this->covariance_model.IsNull() == false)
					throw Exceptions::NoVariableAfterCovarianceModelException();

				// Add intercept
				typedef GlmmGS::Variables::Intercept T;
				this->variables.Add(Pointer<T>(new(bl) T(duplicate)));
			}

			void BlockSection::AddCovariate(const ImmutableVector<int> & values, int duplicate)
			{
				if (this->covariance_model.IsNull() == false)
					throw Exceptions::NoVariableAfterCovarianceModelException();

				// Add covariate
				typedef GlmmGS::Variables::VectorVariable<int> T;
				this->variables.Add(Pointer<T>(new(bl) T(values, duplicate)));
			}

			void BlockSection::AddCovariate(const ImmutableVector<double> & values, int duplicate)
			{
				if (this->covariance_model.IsNull() == false)
					throw Exceptions::NoVariableAfterCovarianceModelException();

				// Add covariate
				typedef GlmmGS::Variables::VectorVariable<double> T;
				this->variables.Add(Pointer<T>(new(bl) T(values, duplicate)));
			}

			void BlockSection::AddIdentityModel(const ImmutableVector<double> & theta)
			{
				if (this->covariance_model.IsNull() == false)
					throw Exceptions::MultipleCovarianceModelsException();

				// Add covariance model
				typedef GlmmGS::RandomEffects::Global::CovarianceModels::IdentityModel T;
				this->covariance_model = Pointer<T>(new(bl) T(this->variables.Size(), theta));
			}

			void BlockSection::AddPrecisionModel(const ImmutableMatrix<double> & R, const ImmutableVector<double> & theta)
			{
				if (this->covariance_model.IsNull() == false)
					throw Exceptions::MultipleCovarianceModelsException();

				// Add covariance model
				typedef GlmmGS::RandomEffects::Global::CovarianceModels::PrecisionModel T;
				this->covariance_model = Pointer<T>(new(bl) T(R, theta));
			}

			void BlockSection::EndBlock()
			{
				if (this->covariance_model.IsNull() == true)
					throw Exceptions::NoCovarianceModelException();

				// Add random effect block to random effects;
				typedef GlmmGS::RandomEffects::Global::Block T;
				this->data->random_effects.Add(Pointer<T>(new(bl) T(this->variables.ToVector(), this->covariance_model)));
			}
		}
	}
}
