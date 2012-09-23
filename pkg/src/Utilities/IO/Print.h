#ifndef UTILITIES_IO_PRINT_H
#define UTILITIES_IO_PRINT_H

#ifdef _STANDALONE
#include <stdio.h>
#define _printf printf
#else
#include "R_ext/Print.h"
#define _printf Rprintf
#endif

namespace Utilities
{
	namespace IO
	{
		// Basic print functions
		void Print(const char * format);
		template <class T> void Print(const char * format, T value);
		template <class T1, class T2> void Print(const char * format, T1 value1, T2 value2);
		template <class T1, class T2, class T3> void Print(const char * format, T1 value1, T2 value2, T3 value3);

		// Definition
		inline
		void Print(const char * format)
		{
			_printf(format);
		}

		template <class T> inline
		void Print(const char * format, T value)
		{
			_printf(format, value);
		}

		template <class T1, class T2> inline
		void Print(const char * format, T1 value1, T2 value2)
		{
			_printf(format, value1, value2);
		}

		template <class T1, class T2, class T3> inline
		void Print(const char * format, T1 value1, T2 value2, T3 value3)
		{
			_printf(format, value1, value2, value3);
		}
	}
}
#endif
