#include "../../Standard.h"
#include "../../GlmmGSAPI.h"
#include "../../Exceptions/Exceptions.h"
#include "BlockSection.h"

namespace GlmmGSAPI
{
	namespace RandomEffects
	{
		namespace Stratified
		{
			// BlockSection
			BlockSection::BlockSection(const Section & section, WeakFactor factor)
				: Section(section), factor(factor), booster(new(bl) GlmmGS::Boosters::Default)
			{
			}

			void BlockSection::AddIntercept(int duplicate)
			{
				if (this->covariance_model.IsNull() == false)
					throw Exceptions::NoVariableAfterCovarianceModelException();

				if (this->data->fixed_intercept == true)
					this->booster.Reset(new(bl) GlmmGS::Boosters::RemoveMean);

				// Add intercept
				typedef GlmmGS::Variables::Intercept T;
				this->variables.Add(Pointer<T>(new(bl) T(duplicate)));
			}

			void BlockSection::AddCovariate(Vector<const int> values, int duplicate)
			{
				if (this->covariance_model.IsNull() == false)
					throw Exceptions::NoVariableAfterCovarianceModelException();

				// Add covariate
				typedef GlmmGS::Variables::VectorVariable<const int> T;
				this->variables.Add(Pointer<T>(new(bl) T(values, duplicate)));
			}

			void BlockSection::AddCovariate(Vector<const double> values, int duplicate)
			{
				if (this->covariance_model.IsNull() == false)
					throw Exceptions::NoVariableAfterCovarianceModelException();

				// Add covariate
				typedef GlmmGS::Variables::VectorVariable<const double> T;
				this->variables.Add(Pointer<T>(new(bl) T(values, duplicate)));
			}

			void BlockSection::AddIdentityCovarianceModel(Matrix<const double> S)
			{
				if (this->covariance_model.IsNull() == false)
					throw Exceptions::MultipleCovarianceModelsException();

				// Add covariance model
				typedef GlmmGS::RandomEffects::Stratified::CovarianceModels::IdentityModel T;
				this->covariance_model.Reset(new(bl) T(this->variables.Size(), this->factor.NumberOfLevels(), S));
			}

			void BlockSection::AddPrecisionModel(Matrix<const double> R, Matrix<const double> S)
			{
				if (this->covariance_model.IsNull() == false)
					throw Exceptions::MultipleCovarianceModelsException();

				// Add covariance model
				typedef GlmmGS::RandomEffects::Stratified::CovarianceModels::PrecisionModel T;
				this->covariance_model.Reset(new(bl) T(this->variables.Size(), R, S));
			}

			void BlockSection::AddSparsePrecisionModel(const LDL::SparseMatrix<double> & R, Matrix<const double> S)
			{
				if (this->covariance_model.IsNull() == false)
					throw Exceptions::MultipleCovarianceModelsException();

				// Add sparse covariance model
				typedef GlmmGS::RandomEffects::Stratified::CovarianceModels::SparsePrecisionModel T;
				this->covariance_model.Reset(new(bl) T(this->variables.Size(), R, S));
			}

			void BlockSection::EndStratifiedBlock()
			{
				if (this->covariance_model.IsNull() == true)
					throw Exceptions::NoCovarianceModelException();

				// Add random effect block to random effects;
				typedef GlmmGS::RandomEffects::Stratified::Block T;
				this->data->random_effects.Add(Pointer<T>(new(bl) T(this->variables.ToVector(),
						this->factor, this->covariance_model, this->booster)));
			}
		}
	}
}
