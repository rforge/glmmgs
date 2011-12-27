#ifndef UTILITIES_NEW_H
#define UTILITIES_NEW_H

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
	class PODAllocator
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
			return NULL;
		}

		static void Delete(TYPE * ptr)
		{
			return delete [] ptr;
		}
	};

	// NewAllocator
	template <class TYPE>
	class NewAllocator
	{
	public:
		static TYPE * New(int size)
		{
			return size > 0 ? new(bl) TYPE[size] : NULL;
		}

		static void Delete(TYPE * ptr)
		{
			return delete [] ptr;
		}
	};

	// Specialization for POD
	template <> class NewAllocator<char> : public PODAllocator<char> {};
	template <> class NewAllocator<unsigned char> : public PODAllocator<unsigned char> {};
	template <> class NewAllocator<int> : public PODAllocator<int> {};
	template <> class NewAllocator<unsigned int> : public PODAllocator<unsigned int> {};
	template <> class NewAllocator<float> : public PODAllocator<float> {};
	template <> class NewAllocator<double> : public PODAllocator<double> {};
}

#endif
