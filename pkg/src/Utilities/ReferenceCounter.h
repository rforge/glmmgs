#ifndef UTILITIES_REFERENCECOUNTER_H
#define UTILITIES_REFERENCECOUNTER_H

#include "New.h"
#include <stdlib.h>

namespace Utilities
{
	// ReferenceCounter
	class ReferenceCounter
	{
	private:
		// Disable assignment
		const ReferenceCounter & operator =(const ReferenceCounter &);

		// Fields
		int * count;

	public:
		// Construction
		ReferenceCounter();
		explicit ReferenceCounter(const void * ptr);
		ReferenceCounter(const ReferenceCounter & counter);

		// Attributes
		int Count() const;
		
		// Methods
		void Reset(void * ptr);
		void Increment(const ReferenceCounter & counter);
		int Decrement();
	};

	// Construction
	inline
	ReferenceCounter::ReferenceCounter()
		: count(NULL)
	{
	}

	inline
	ReferenceCounter::ReferenceCounter(const void * ptr)
		: count(ptr != NULL ? new(bl) int(1) : NULL)
	{
	}

	inline
	ReferenceCounter::ReferenceCounter(const ReferenceCounter & refCounter)
		: count(refCounter.count)
	{
		if (this->count != NULL)
			++(*this->count);
	}

	// Attributes
	inline
	int ReferenceCounter::Count() const
	{
		return this->count ? *this->count : 0;
	}
}

#endif
