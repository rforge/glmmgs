#include "ReferenceCounter.h"
#include "Exceptions/Assertions.h"

namespace Utilities
{
	void ReferenceCounter::Reset(void * ptr)
	{
		// This method should be called only after Decrement
		_ASSERT_ARGUMENT(this->count == NULL);
		if (ptr != NULL)
			this->count = new(bl) int(1);
	}

	void ReferenceCounter::Increment(const ReferenceCounter & counter)
	{
		// This method should only be called after Decrement
		_ASSERT_ARGUMENT(this->count == NULL);
		if (counter.count != NULL)
		{
			this->count = counter.count;
			++(*this->count);
		}
	}
	
	int ReferenceCounter::Decrement()
	{
		if (this->count != NULL)
		{
			const int count = --(*this->count);
			if (count == 0)
				delete this->count;
			this->count = NULL;
			return count;
		}
		else
		{
			return -1;
		}
	}
}
