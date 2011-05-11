#include "ReferenceCounter.h"
#include "Exceptions/Assertions.h"

namespace Utilities
{
	void ReferenceCounter::SetReference(int count)
	{
		GLMMGS_ASSERT_ARGUMENT(this->counter == NULL)
		this->counter = new(bl) int;
		*this->counter = count;
	}

	void ReferenceCounter::AddReference(const ReferenceCounter & refCounter)
	{
		GLMMGS_ASSERT_ARGUMENT(this->counter == NULL)
		this->counter = refCounter.counter;
		++(*this->counter);
	}
	
	int ReferenceCounter::RemoveReference()
	{
		int count = -1;
		if (this->counter != NULL)
		{
			count = --(*this->counter);
			if (count == 0)
				delete this->counter;
			this->counter = NULL;
		}
		return count;
	}
}
