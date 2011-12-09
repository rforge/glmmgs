#include "ReferenceCounter.h"
#include "Exceptions/Assertions.h"

namespace Utilities
{
	void ReferenceCounter::SetReference(int count)
	{
		_ASSERT_ARGUMENT(this->counter == NULL)
		this->counter = new(bl) int(count);
	}

	void ReferenceCounter::AddReference(const ReferenceCounter & refCounter)
	{
		_ASSERT_ARGUMENT(this->counter == NULL)
		this->counter = refCounter.counter;
		++(*this->counter);
	}
	
	int ReferenceCounter::RemoveReference()
	{
		if (this->counter != NULL)
		{
			const int count = --(*this->counter);
			if (count == 0)
				delete this->counter;
			this->counter = NULL;
			return count;
		}
		else
		{
			return -1;
		}
	}
}
