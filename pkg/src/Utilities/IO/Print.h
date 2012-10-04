#ifndef UTILITIES_IO_PRINT_H
#define UTILITIES_IO_PRINT_H

#ifdef _CONSOLE
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
		// Print functions
		void Print(const char * format);

		template <class T> void Print(const char * format, T value);
		template <class T1, class T2> void Print(const char * format, T1 value1, T2 value2);
		template <class T1, class T2, class T3> void Print(const char * format, T1 value1, T2 value2, T3 value3);
		template <class T1, class T2, class T3, class T4> void Print(const char * format, T1 value1, T2 value2, T3 value3, T4 value4);

		template <class TYPE> void Print(const char * format, const Vector<TYPE> & values);
		template <class TYPE> void Print(const char * format, const ImmutableVector<TYPE> & values);
		template <class TYPE> void Print(const char * format, const ImmutableVector<Vector<TYPE> > & values);
		template <class TYPE> void Print(const char * format, const Matrix<TYPE> & matrix);
		template <class TYPE> void Print(const char * format, const ImmutableMatrix<TYPE> & matrix);
		template <class TYPE> void Print(const char * format, const TriangularMatrix<TYPE> & matrix);
		template <class TYPE> void Print(const char * format, const ImmutableTriangularMatrix<TYPE> & matrix);

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

		template <class T1, class T2, class T3, class T4> inline
		void Print(const char * format, T1 value1, T2 value2, T3 value3, T4 value4)
		{
			_printf(format, value1, value2, value3, value4);
		}

		// Print vector
		template <class TYPE>
		void Print(const char * format, const Vector<TYPE> & values)
		{
			Print(format, static_cast<const ImmutableVector<TYPE> & >(values));
		}

		template <class TYPE>
		void Print(const char * format, const ImmutableVector<TYPE> & values)
		{
			const int n = values.Size() - 1;
			for (int j = 0; j < n; ++j)
			{
				Print(format, values(j));
				Print(" ", values(j));
			}
			Print(format, values(n));
			Print("\n");
		}

		// Print vector of vectors
		template <class TYPE>
		void Print(const char * format, const ImmutableVector<Vector<TYPE> > & values)
		{
			for (int j = 0; j < values.Size(); ++j)
				Print(format, values(j));
		}

		// Print Matrix
		template <class TYPE>
		void Print(const char * format, const Matrix<TYPE> & matrix)
		{
			Print(format, static_cast<const ImmutableMatrix<TYPE> &>(matrix));
		}

		template <class TYPE>
		void Print(const char * format, const ImmutableMatrix<TYPE> & matrix)
		{
			for (int i = 0; i < matrix.NumberOfRows(); ++i)
			{
				const int m = matrix.NumberOfColumns() - 1;
				for (int j = 0; j < m; ++j)
				{
					Print(format, matrix(i, j));
					Print(" ");
				}
				Print(format, matrix(i, i));
				Print("\n");
			}
		}

		// Print TriangularMatrix
		template <class TYPE>
		void Print(const char * format, const TriangularMatrix<TYPE> & matrix)
		{
			Print(format, static_cast<const ImmutableTriangularMatrix<TYPE> &>(matrix));
		}

		template <class TYPE>
		void Print(const char * format, const ImmutableTriangularMatrix<TYPE> & matrix)
		{
			for (int i = 0; i < matrix.NumberOfRows(); ++i)
			{
				for (int j = 0; j < i; ++j)
				{
					Print(format, matrix(i, j));
					Print(" ");
				}
				Print(format, matrix(i, i));
				Print("\n");
			}
		}
	}
}

#endif
