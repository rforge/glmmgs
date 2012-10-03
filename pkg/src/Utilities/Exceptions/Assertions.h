#ifndef UTILITIES_EXCEPTIONS_ASSERTIONS_H
#define UTILITIES_EXCEPTIONS_ASSERTIONS_H

#include "Exception.h"

#ifdef _DEBUG
	#define _STR(str) #str
	#define _ASSERT(expression) \
		if ((expression) == false) \
			throw Utilities::Exceptions::Exception("\nAssertion failed: " _STR(expression) "\nFile: " __FILE__ "\n")
#else
	#define _ASSERT(expression) (void(0));
#endif

#endif
