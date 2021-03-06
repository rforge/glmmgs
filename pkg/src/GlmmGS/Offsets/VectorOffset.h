#ifndef GLMMGS_OFFSETS_WEAKVECTOROFFSET_H
#define GLMMGS_OFFSETS_WEAKVECTOROFFSET_H

#include "../Standard.h"
#include "IOffset.h"
namespace GlmmGS
{
	namespace Offsets
	{
		// VectorOffset
		template <class TYPE>
		class VectorOffset : public IOffset
		{
		private:
			ImmutableVector<TYPE> values;

		public:
			// Construction
			VectorOffset(const ImmutableVector<TYPE> & values);
			~VectorOffset();

			// Methods
			void InitializePredictor(Vector<double> & eta) const;
		};

		// Construction
		template <class TYPE>
		VectorOffset<TYPE>::VectorOffset(const ImmutableVector<TYPE> & values)
			: values(values)
		{
		}

		template <class TYPE>
		VectorOffset<TYPE>::~VectorOffset()
		{
		}

		// Methods
		template <class TYPE>
		void VectorOffset<TYPE>::InitializePredictor(Vector<double> & eta) const
		{
			_ASSERT(eta.Size() == this->values.Size());
			for (int i = 0; i < this->values.Size(); ++i)
				eta(i) = this->values(i);
		}
	}
}

#endif
