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
			Vector<TYPE> values;

		public:
			// Construction
			VectorOffset(Vector<TYPE> values);
			~VectorOffset();

			// Methods
			void InitializePredictor(Vector<double> & eta) const;
		};

		// Construction
		template <class TYPE>
		VectorOffset<TYPE>::VectorOffset(Vector<TYPE> values)
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
			_ASSERT_ARGUMENT(eta.Size() == this->values.Size());
			for (int i = 0; i < this->values.Size(); ++i)
				eta(i) = this->values(i);
		}
	}
}

#endif
