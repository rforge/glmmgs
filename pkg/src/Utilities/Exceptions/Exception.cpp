#include <stdlib.h>
#include <memory.h>
#include "Exception.h"

namespace Utilities
{
	namespace Exceptions
	{
		// Construction
		Exception::Exception(const char * message)
			: count(0)
		{
			// Initialize buffer
			for (int i = 0; i < this->capacity; ++i)
				this->buffer[i] = '\0';

			// Push message
			*this << message;
		}

		// Message
		const char * Exception::Message() const
		{
			return buffer;
		}

		// Inserter
		Exception & Exception::operator << (const char * message)
		{
			if (message)
			{
				// Set max length
				const int max_length = this->capacity - this->count - 1;

				// Copy message to buffer
				for (int i = 0; i < max_length; ++i, ++this->count)
				{
					const char c = message[i];
					if (c == '\0')
						break;
					this->buffer[this->count] = c;
				}
				
				// Terminate buffer
				this->buffer[this->count] = '\0'; 
			}

			return *this;
		}
	}
}
