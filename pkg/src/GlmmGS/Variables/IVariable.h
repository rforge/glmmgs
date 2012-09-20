#ifndef GLMMGS_UTILITIES_VARIABLES_IVARIABLE_H
#define GLMMGS_UTILITIES_VARIABLES_IVARIABLE_H

#include "../Standard.h"

namespace GlmmGS
{
	namespace Variables
	{
		// IVariable
		class IVariable
		{
			// Friends
			friend class Intercept;
			friend class StratifiedIntercept;
			template <class TYPE> friend class VectorVariable;
			friend double ScalarProduct(const Pointer<IVariable> & variable, const ImmutableVector<double> & values);
			friend double ScalarProduct(const Pointer<IVariable> & variable1, const ImmutableVector<double> & weights, const Pointer<IVariable> & variable2);
			friend Utilities::Vector<double> ScalarProduct(const Pointer<IVariable> & variable, const ImmutableVector<double> & values, WeakFactor factor);
			friend Utilities::Vector<double> ScalarProduct(const Pointer<IVariable> & variable1, const ImmutableVector<double> & weights, const Pointer<IVariable> & variable2, WeakFactor factor);

		private:
			// Operations
			virtual double ScalarProduct(const ImmutableVector<double> & values) const = 0;
			virtual double ScalarProduct(const ImmutableVector<double> & weights, const Pointer<IVariable> & variable) const = 0;
			virtual double ScalarProduct(const ImmutableVector<double> & weights, const ImmutableVector<int> & values) const = 0;
			virtual double ScalarProduct(const ImmutableVector<double> & weights, const ImmutableVector<double> & values) const = 0;

			// Stratified Operations
			virtual Vector<double> ScalarProduct(const ImmutableVector<double> & values, WeakFactor factor) const = 0;
			virtual Vector<double> ScalarProduct(const ImmutableVector<double> & weights, const Pointer<IVariable> & variable, WeakFactor factor) const = 0;
			virtual Vector<double> ScalarProduct(const ImmutableVector<double> & weights, const ImmutableVector<int> & values, WeakFactor factor) const = 0;
			virtual Vector<double> ScalarProduct(const ImmutableVector<double> & weights, const ImmutableVector<double> & values, WeakFactor factor) const = 0;

		public:
			// Properties
			const int duplicate;

		protected:
			// Construction
			IVariable(int duplicate);

		public:
			virtual ~IVariable();

			// Methods
			virtual void UpdatePredictor(Vector<double> & eta, double beta) const = 0;
			// Stratified methods
			virtual void UpdatePredictor(Vector<double> & eta, const ImmutableVector<double> & beta, WeakFactor factor) const = 0;
		};

		// Operations (friend functions)
		inline
		double ScalarProduct(const Pointer<IVariable> & variable, const ImmutableVector<double> & values)
		{
			return variable->ScalarProduct(values);
		}
	
		inline
		double ScalarProduct(const Pointer<IVariable> & variable1, const ImmutableVector<double> & weights, const Pointer<IVariable> & variable2)
		{
			return variable1->ScalarProduct(weights, variable2);
		}
	
		// Stratified Operations (friend functions)
		inline
		Vector<double> ScalarProduct(const Pointer<IVariable> & variable, const ImmutableVector<double> & values, WeakFactor factor)
		{
			return variable->ScalarProduct(values, factor);
		}

		inline
		Vector<double> ScalarProduct(const Pointer<IVariable> & variable1, const ImmutableVector<double> & weights, const Pointer<IVariable> & variable2, WeakFactor factor)
		{
			return variable1->ScalarProduct(weights, variable2, factor);
		}
	}
}

#endif
