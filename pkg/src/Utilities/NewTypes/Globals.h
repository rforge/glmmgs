#include "../Exceptions/Assertions.h"
#include "Vector.h"

namespace Utilities
{
	namespace NewTypes
	{
		// Copy vector
		template <class DST_TYPE, class SRC_TYPE>
		void CopyVector(Vector<DST_TYPE> & dst,	const Vector<SRC_TYPE> & src)
		{
			_ASSERT_ARGUMENT(dst.Size() == src.Size());
			for (int i = 0; i < dst.Size(); ++i)
				dst(i) = src(i);
		}
	}
}
