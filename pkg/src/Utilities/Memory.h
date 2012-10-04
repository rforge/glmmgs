#ifndef UTILITIES_MEMORY_H
#define UTILITIES_MEMORY_H

#include <stdlib.h>
#include <memory.h>

namespace Utilities
{
	class BLTag{};
	const BLTag bl = BLTag();
}

// Global operator new and delete
void * operator new(size_t size, const Utilities::BLTag &);
void operator delete(void * ptr, const Utilities::BLTag &);
void * operator new [](size_t size, const Utilities::BLTag &);
void operator delete [] (void * ptr, const Utilities::BLTag &);

namespace Utilities
{
	// Plain old data allocator
	template <class TYPE>
	class PodAllocator
	{
	public:
		static TYPE * New(int size)
		{
			if (size > 0)
			{
				TYPE * ptr = new(bl) TYPE[size];
				memset(ptr, 0, size * sizeof(TYPE));
				return ptr;
			}
			else
			{
				return NULL;
			}
		}

		static void Delete(TYPE * ptr)
		{
			delete [] ptr;
		}
	};

	// NewAllocator
	template <class TYPE>
	class Allocator
	{
	public:
		static TYPE * New(int size)
		{
			return size > 0 ? new(bl) TYPE[size] : NULL;
		}

		static void Delete(TYPE * ptr)
		{
			delete [] ptr;
		}
	};

	// Specialization for POD
	template <> class Allocator<char> : public PodAllocator<char> {};
	template <> class Allocator<unsigned char> : public PodAllocator<unsigned char> {};
	template <> class Allocator<int> : public PodAllocator<int> {};
	template <> class Allocator<unsigned int> : public PodAllocator<unsigned int> {};
	template <> class Allocator<float> : public PodAllocator<float> {};
	template <> class Allocator<double> : public PodAllocator<double> {};

	// Reset function
	template <class TYPE> inline
	void Reset(TYPE & obj)
	{
		obj = TYPE();
	}
}

#endif
