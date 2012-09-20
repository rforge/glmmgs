#ifndef UTILITIES_IO_PRINT_OBJECTS_H
#define UTILITIES_IO_PRINT_OBJECTS_H

#include "../Vector.h"
#include "../TriangularMatrix.h"
#include "Print.h"

namespace Utilities
{
	namespace IO
	{
		// Declaration
		template <class TYPE> void Print(const char * format, const ImmutableVector<TYPE> & values);
		template <class TYPE> void Print(const char * format, const Vector<Vector<TYPE> > & values);
		template <class TYPE> void Print(const char * format, const ImmutableMatrix<TYPE> & matrix);
		template <class TYPE> void Print(const char * format, const ImmutableTriangularMatrix<TYPE> & matrix);

		// Definition

		// Print vector
		template <class TYPE>
		void Print(const char * format, const ImmutableVector<TYPE> & values)
		{
			for (int j = 0; j < values.Size(); ++j)
				Print(format, values(j));
		}

		// Print vector of vectors
		template <class TYPE>
		void Print(const char * format, const Vector<Vector<TYPE> > & values)
		{
			for (int j = 0; j < values.Size(); ++j)
				Print(format, values(j));
		}

		// Print Matrix
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
