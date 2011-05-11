#pragma once
#include "../Vector.h"
#include "../TriangularMatrix.h"
#include "../WeakMatrix.h"
#include <stdio.h>

namespace Utilities
{
	namespace IO
	{
		// Stream
		extern FILE * stream;

		// Declaration
		template <class TYPE> void Print(const char * format, TYPE value);
		template <class TYPE> void Print(const char * format, const Vector<TYPE> & values);
		template <class TYPE> void Print(const char * format, const Vector<Vector<TYPE> > & values);
		template <class TYPE> void Print(const char * format, const WeakMatrix<TYPE> matrix);
		template <class TYPE> void Print(const char * format, const TriangularMatrix<TYPE> & matrix);

		// Definition
		template <class TYPE>
		void Print(const char * format, TYPE value)
		{
			fprintf(stream, format, value);
		}

		template <class TYPE>
		void Print(const char * format, const Vector<TYPE> & values)
		{
			for (int j = 0; j < values.Size(); ++j)
				fprintf(stream, format, values(j));
		}

		template <class TYPE>
		void Print(const char * format, const Vector<Vector<TYPE> > & values)
		{
			for (int j = 0; j < values.Size(); ++j)
				Print(format, values(j));
		}

		template <class TYPE>
		void Print(const char * format, const WeakMatrix<TYPE> matrix)
		{
			for (int i = 0; i < matrix.NumberOfRows(); ++i)
			{
				const int m = matrix.NumberOfColumns() - 1;
				for (int j = 0; j < m; ++j)
				{
					Print(format, matrix(i, j));
					printf(" ");
				}
				Print(format, matrix(i, i));
				printf("\n");
			}
		}

		template <class TYPE>
		void Print(const char * format, const TriangularMatrix<TYPE> & matrix)
		{
			for (int i = 0; i < matrix.NumberOfRows(); ++i)
			{
				for (int j = 0; j < i; ++j)
				{
					Print(format, matrix(i, j));
					printf(" ");
				}
				Print(format, matrix(i, i));
				printf("\n");
			}
		}
	}
}
