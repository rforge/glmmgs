#ifndef UTILITIES_IMMUTABLE_H
#define UTILITIES_IMMUTABLE_H

namespace Utilities
{
	template <class TYPE>
	struct Immutable
	{
		typedef const TYPE Type;
	};
}

#endif
