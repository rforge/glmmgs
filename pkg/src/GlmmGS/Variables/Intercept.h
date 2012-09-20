#ifndef GLMMGS_UTILITIES_VARIABLES_INTERCEPT_H
#define GLMMGS_UTILITIES_VARIABLES_INTERCEPT_H

#include "../Standard.h"
#include "IVariable.h"

namespace GlmmGS
{
	namespace Variables
	{
		// Intercept
		class Intercept : public IVariable
		{
		private:
			// Operations
			void UpdatePredictor(Vector<double> & eta, double beta) const;
			double ScalarProduct(const ImmutableVector<double> & values) const;
			double ScalarProduct(const ImmutableVector<double> & weights, const Pointer<IVariable> & variable) const;
			double ScalarProduct(const ImmutableVector<double> & weights, const ImmutableVector<int> & values) const;
			double ScalarProduct(const ImmutableVector<double> & weights, const ImmutableVector<double> & values) const;
			template <class OTHER> double ScalarProductImpl(const ImmutableVector<double> & weights, const ImmutableVector<OTHER> & values) const;

			// Stratified Operations
			void UpdatePredictor(Vector<double> & eta, const ImmutableVector<double> & beta, WeakFactor factor) const;
			Vector<double> ScalarProduct(const ImmutableVector<double> & values, WeakFactor factor) const;
			Vector<double> ScalarProduct(const ImmutableVector<double> & weights, const Pointer<IVariable> & variable, WeakFactor factor) const;
			Vector<double> ScalarProduct(const ImmutableVector<double> & weights, const ImmutableVector<int> & values, WeakFactor factor) const;
			Vector<double> ScalarProduct(const ImmutableVector<double> & weights, const ImmutableVector<double> & values, WeakFactor factor) const;
			template <class OTHER> Vector<double> ScalarProductImpl(const ImmutableVector<double> & weights, const ImmutableVector<OTHER> & values, WeakFactor factor) const;

		public:
			// Construction
			Intercept(int duplicate);
			~Intercept();
		};

		// Operations
		inline
		double Intercept::ScalarProduct(const ImmutableVector<double> & weights, const Pointer<IVariable> & variable) const
		{
			// Double dispatch
			return variable->ScalarProduct(weights);
		}

		inline
		double Intercept::ScalarProduct(const ImmutableVector<double> & weights, const ImmutableVector<int> & values) const
		{
			return this->ScalarProductImpl(weights, values);
		}

		inline
		double Intercept::ScalarProduct(const ImmutableVector<double> & weights, const ImmutableVector<double> & values) const
		{
			return this->ScalarProductImpl(weights, values);
		}

		template <class OTHER>
		double Intercept::ScalarProductImpl(const ImmutableVector<double> & weights, const ImmutableVector<OTHER> & values) const
		{
			_ASSERT_ARGUMENT(weights.Size() == values.Size());
			const int n = values.Size();
			double sum = 0.0;
			for (int i = 0; i < n; ++i)
				sum += weights(i) * values(i);
			return sum;
		}
	
		// Stratified Operations
		inline
		Vector<double> Intercept::ScalarProduct(const ImmutableVector<double> & weights, const Pointer<IVariable> & variable, WeakFactor factor) const
		{
			// Double dispatch
			return variable->ScalarProduct(weights, factor);
		}

		inline
		Vector<double> Intercept::ScalarProduct(const ImmutableVector<double> & weights, const ImmutableVector<int> & values, WeakFactor factor) const
		{
			return this->ScalarProductImpl(weights, values, factor);
		}

		inline
		Vector<double> Intercept::ScalarProduct(const ImmutableVector<double> & weights, const ImmutableVector<double> & values, WeakFactor factor) const
		{
			return this->ScalarProductImpl(weights, values, factor);
		}
	
		template <class OTHER>
		Vector<double> Intercept::ScalarProductImpl(const ImmutableVector<double> & weights, const ImmutableVector<OTHER> & values, WeakFactor factor) const
		{
			_ASSERT_ARGUMENT(weights.Size() == values.Size() && factor.Size() == values.Size());
			const int n = values.Size();
			const int nlevels = factor.NumberOfLevels();
			Vector<double> sum(nlevels);
			for (int i = 0; i < n; ++i)
			{
				const int level = factor.Level(i);
				sum(level) += weights(i) * values(i);
			}
			return sum;
		}
	}
}

#endif
