#ifndef UTILITIES_IO_PRINT_H
#define UTILITIES_IO_PRINT_H

#ifdef _STANDALONE
#include <stdio.h>
#else
#include "C:/Program Files/R/R-2.15.1/include/R_ext/Print.h"
#endif

namespace Utilities
{
	namespace IO
	{
		// Basic print functions
		void Print(const char * format);
		template <class TYPE> void Print(const char * format, TYPE value);

#ifdef _STANDALONE
		// Definition
		inline
		void Print(const char * format)
		{
			fprintf(stdout, format);
			fflush(stdout);
		}

		template <class TYPE> inline
		void Print(const char * format, TYPE value)
		{
			fprintf(stdout, format, value);
			fflush(stdout);
		}
#else
		inline
		void Print(const char * format)
		{
			Rprintf(format);
		}

		template <class TYPE> inline
		void Print(const char * format, TYPE value)
		{
			Rprintf(format, value);
		}
#endif
	}
}
#endif
