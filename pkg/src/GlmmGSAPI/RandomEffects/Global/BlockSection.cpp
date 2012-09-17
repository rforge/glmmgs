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
			void BlockSection::AddIntercept()
			{
				if (this->covariance_model.IsNull() == false)
					throw Exceptions::NoVariableAfterCovarianceModelException();

				// Add intercept
				typedef GlmmGS::Variables::Intercept T;
				this->variables.Add(Pointer<T>(new(bl) T));
			}

			void BlockSection::AddCovariate(Vector<const int> values)
			{
				if (this->covariance_model.IsNull() == false)
					throw Exceptions::NoVariableAfterCovarianceModelException();

				// Add covariate
				typedef GlmmGS::Variables::VectorVariable<const int> T;
				this->variables.Add(Pointer<T>(new(bl) T(values)));
			}

			void BlockSection::AddCovariate(Vector<const double> values)
			{
				if (this->covariance_model.IsNull() == false)
					throw Exceptions::NoVariableAfterCovarianceModelException();

				// Add covariate
				typedef GlmmGS::Variables::VectorVariable<const double> T;
				this->variables.Add(Pointer<T>(new(bl) T(values)));
			}

			void BlockSection::AddIdentityCovarianceModel(Matrix<const double> S)
			{
				if (this->covariance_model.IsNull() == false)
					throw Exceptions::MultipleCovarianceModelsException();

				// Add covariance model
				typedef GlmmGS::RandomEffects::Global::CovarianceModels::IdentityModel T;
				this->covariance_model.Reset(new(bl) T(this->variables.Size(), S));
			}

			void BlockSection::AddPrecisionModel(Matrix<const double> R, Matrix<const double> S)
			{
				if (this->covariance_model.IsNull() == false)
					throw Exceptions::MultipleCovarianceModelsException();

				// Add covariance model
				typedef GlmmGS::RandomEffects::Global::CovarianceModels::PrecisionModel T;
				this->covariance_model.Reset(new(bl) T(R, S));
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
