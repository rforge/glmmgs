#pragma once

#include "New.h"
#include <stdlib.h>

namespace Utilities
{
	// ReferenceCounter
	class ReferenceCounter
	{
	public:
		// Attributes
		int Count() const;

	protected:
		// Construction
		ReferenceCounter();
		ReferenceCounter(int count); // Set reference
		ReferenceCounter(const ReferenceCounter & refCounter); // Add reference
		
		// Methods
		void SetReference(int count);
		void AddReference(const ReferenceCounter & refCounter);
		int RemoveReference();

	private:
		const ReferenceCounter & operator =(const ReferenceCounter &);
		
		// Data
		int * counter;
	};

	// Definition
	inline
	int ReferenceCounter::Count() const
	{
		return this->counter ? *this->counter : 0;
	}
	
	inline
	ReferenceCounter::ReferenceCounter()
		: counter(NULL)
	{
	}

	inline
	ReferenceCounter::ReferenceCounter(int count)
		: counter(new(bl) int(count))
	{
	}

	inline
	ReferenceCounter::ReferenceCounter(const ReferenceCounter & refCounter)
		: counter(refCounter.counter)
	{
		if (this->counter != NULL)
			++(*this->counter);
	}
}
