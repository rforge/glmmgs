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
				: Section(section), factor(factor)
			{
			}

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

			void BlockSection::AddIdentityCovarianceModel(const ImmutableMatrix<double> & S)
			{
				if (this->covariance_model.IsNull() == false)
					throw Exceptions::MultipleCovarianceModelsException();

				// Add covariance model
				typedef GlmmGS::RandomEffects::Stratified::CovarianceModels::IdentityModel T;
				this->covariance_model.Reset(new(bl) T(this->variables.Size(), this->factor.NumberOfLevels(), S));
			}

			void BlockSection::AddPrecisionModel(const ImmutableMatrix<double> & R, const ImmutableMatrix<double> & S)
			{
				if (this->covariance_model.IsNull() == false)
					throw Exceptions::MultipleCovarianceModelsException();

				// Add covariance model
				typedef GlmmGS::RandomEffects::Stratified::CovarianceModels::PrecisionModel T;
				this->covariance_model.Reset(new(bl) T(this->variables.Size(), R, S));
			}

			void BlockSection::AddSparsePrecisionModel(const LDL::SparseMatrix<double> & R, const ImmutableMatrix<double> & S)
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
				this->data->random_effects.Add(Pointer<T>(
						new(bl) T(this->variables.ToVector(), this->factor, this->covariance_model)));
			}
		}
	}
}
