#ifndef UTILITIES_COLLECTIONS_BUFFER_H
#define UTILITIES_COLLECTIONS_BUFFER_H

#include "Container.h"

namespace Utilities
{
	namespace Collections
	{
		// Buffer
		class Buffer : public Container<char>
		{
		private:
			// Fields
			int length;

		public:
			// Construction
			Buffer(int capacity);

			// Properties
			const char * Pointer() const;
			int Length() const;

			// Operations
			void Append(const char * msg);
			void Empty();
		};

		// Definition
		inline
		const char * Buffer::Pointer() const
		{
			return this->ptr;
		}

		inline
		int Buffer::Length() const
		{
			return this->length;
		}
	}
}

#endif
