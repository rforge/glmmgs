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
		ReferenceCounter(const ReferenceCounter & counter);
		explicit ReferenceCounter(void * ptr);

		// Attributes
		int Count() const;
		
		// Methods
		void Initialize(void * ptr);
		void Attach(const ReferenceCounter & counter);
		int Detach();
	};

	// Construction
	inline
	ReferenceCounter::ReferenceCounter()
		: count(NULL)
	{
	}

	inline
	ReferenceCounter::ReferenceCounter(const ReferenceCounter & refCounter)
		: count(refCounter.count)
	{
		if (this->count != NULL)
			++(*this->count);
	}

	inline
	ReferenceCounter::ReferenceCounter(void * ptr)
		: count(ptr != NULL ? new(bl) int(1) : NULL)
	{
	}

	// Attributes
	inline
	int ReferenceCounter::Count() const
	{
		return this->count ? *this->count : 0;
	}
}

#endif
