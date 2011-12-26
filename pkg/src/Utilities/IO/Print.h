#ifndef UTILITIES_IO_PRINT_H
#define UTILITIES_IO_PRINT_H

#ifdef _STANDALONE
#include <stdio.h>
#include "Stream.h"
#else
#include "R_ext/Print.h"
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
			fprintf(stream, format);
			fflush(stream);
		}

		template <class TYPE> inline
		void Print(const char * format, TYPE value)
		{
			fprintf(stream, format, value);
			fflush(stream);
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
