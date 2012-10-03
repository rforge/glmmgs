#pragma once

namespace Utilities
{
	namespace Exceptions
	{
		// Exception
		class Exception
		{
		public:
			static const int capacity = 1024;

		private:
			// Buffer
			char buffer[capacity];
			int count;
		
		public:
			// Construction
			Exception(const char * message = "Exception");
			
			// Message
			const char * Message() const; 
			
			// Inserter
			Exception & operator << (const char * message);
		};
	}
}
