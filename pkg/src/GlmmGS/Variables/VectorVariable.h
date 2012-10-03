#ifndef GLMMGS_UTILITIES_VARIABLES_WEAKVECTORVARIABLE_H
#define GLMMGS_UTILITIES_VARIABLES_WEAKVECTORVARIABLE_H

#include "../Standard.h"
#include "IVariable.h"

namespace GlmmGS
{
	namespace Variables
	{
		// VectorVariable
		template <class TYPE>
		class VectorVariable : public IVariable
		{
		private:
			// Fields
			ImmutableVector<TYPE> values;

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
			VectorVariable(const ImmutableVector<TYPE> & values, int duplicate);
			~VectorVariable();
		};

		// Construction
		template <class TYPE>
		VectorVariable<TYPE>::VectorVariable(const ImmutableVector<TYPE> & values, int duplicate)
			: IVariable(duplicate), values(values)
		{
		}

		template <class TYPE>
		VectorVariable<TYPE>::~VectorVariable()
		{
		}

		// Operations
		template <class TYPE>
		void VectorVariable<TYPE>::UpdatePredictor(Vector<double> & eta, double beta) const
		{
			_ASSERT(eta.Size() == this->values.Size());
			for (int i = 0; i < this->values.Size(); ++i)
				eta(i) += this->values(i) * beta;
		}

		template <class TYPE>
		double VectorVariable<TYPE>::ScalarProduct(const ImmutableVector<double> & values) const
		{
			_ASSERT(values.Size() == this->values.Size());
			double sum = 0.0;
			for (int i = 0; i < this->values.Size(); ++i)
				sum += this->values(i) * values(i);
			return sum;
		}

		template <class TYPE> inline
		double VectorVariable<TYPE>::ScalarProduct(const ImmutableVector<double> & weights, const Pointer<IVariable> & variable) const
		{
			// Double dispatch
			return variable->ScalarProduct(weights, this->values);
		}

		template <class TYPE>
		double VectorVariable<TYPE>::ScalarProduct(const ImmutableVector<double> & weights, const ImmutableVector<int> & values) const
		{
			return this->ScalarProductImpl(weights, values);
		}

		template <class TYPE> inline
		double VectorVariable<TYPE>::ScalarProduct(const ImmutableVector<double> & weights, const ImmutableVector<double> & values) const
		{
			return this->ScalarProductImpl(weights, values);
		}

		template <class TYPE> template <class OTHER>
		double VectorVariable<TYPE>::ScalarProductImpl(const ImmutableVector<double> & weights, const ImmutableVector<OTHER> & values) const
		{
			_ASSERT(weights.Size() == this->values.Size() && values.Size() == this->values.Size());
			double sum = 0.0;
			for (int i = 0; i < this->values.Size(); ++i)
				sum += this->values(i) * weights(i) * values(i);
			return sum;
		}

		// Stratified operations
		template <class TYPE>
		void VectorVariable<TYPE>::UpdatePredictor(Vector<double> & eta, const ImmutableVector<double> & beta, WeakFactor factor) const
		{
			_ASSERT(eta.Size() == this->values.Size() && factor.Size() == this->values.Size() && beta.Size() == factor.NumberOfLevels());
			for (int i = 0; i < this->values.Size(); ++i)
			{
				const int level = factor.Level(i);
				eta(i) += this->values(i) * beta(level);
			}
		}

		template <class TYPE>
		Vector<double> VectorVariable<TYPE>::ScalarProduct(const ImmutableVector<double> & values, WeakFactor factor) const
		{
			_ASSERT(values.Size() == this->values.Size() && factor.Size() == this->values.Size());
			const int nlevels = factor.NumberOfLevels();
			Vector<double> sum(nlevels);
			for (int i = 0; i < this->values.Size(); ++i)
			{
				const int level = factor.Level(i);
				sum(level) += this->values(i) * values(i);
			}
			return sum;
		}

		template <class TYPE> inline
		Vector<double> VectorVariable<TYPE>::ScalarProduct(const ImmutableVector<double> & weights, const Pointer<IVariable> & variable, WeakFactor factor) const
		{
			// Double dispatch
			return variable->ScalarProduct(weights, this->values, factor);
		}

		template <class TYPE> inline
		Vector<double> VectorVariable<TYPE>::ScalarProduct(const ImmutableVector<double> & weights, const ImmutableVector<int> & values, WeakFactor factor) const
		{
			return this->ScalarProductImpl(weights, values, factor);
		}

		template <class TYPE> inline
		Vector<double> VectorVariable<TYPE>::ScalarProduct(const ImmutableVector<double> & weights, const ImmutableVector<double> & values, WeakFactor factor) const
		{
			return this->ScalarProductImpl(weights, values, factor);
		}

		template <class TYPE> template <class OTHER>
		Vector<double> VectorVariable<TYPE>::ScalarProductImpl(const ImmutableVector<double> & weights, const ImmutableVector<OTHER> & values, WeakFactor factor) const
		{
			_ASSERT(weights.Size() == this->values.Size() && values.Size() == this->values.Size() && factor.Size() == this->values.Size());
			const int nlevels = factor.NumberOfLevels();
			Vector<double> sum(nlevels);
			for (int i = 0; i < this->values.Size(); ++i)
			{
				const int level = factor.Level(i);
				sum(level) += this->values(i) * weights(i) * values(i);
			}
			return sum;
		}
	}
}

#endif
