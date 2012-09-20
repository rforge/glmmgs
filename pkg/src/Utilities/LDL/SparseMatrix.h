#ifndef UTILITIES_LDL_SPARSEMATRIX_H
#define UTILITIES_LDL_SPARSEMATRIX_H

#include "Internal/ldl.h"

namespace Utilities
{
	namespace LDL
	{
		// Data structure for SparseCholeskyDecomposition
		template <class TYPE>
		class SparseMatrix
		{
		private:
			// Fields
			Vector<TYPE> values;
			Vector<int> indices;
			Vector<int> counts;

		public:
			// Construction
			SparseMatrix();
			SparseMatrix(Vector<TYPE> values, Vector<int> indices, Vector<int> counts);

			// Properties
			int NumberOfColumns() const;
			TYPE Value(int index) const;
			int Index(int index) const;
			int Count(int col) const;
			const ImmutableVector<TYPE> & Values() const;
			const ImmutableVector<int> & Indices() const;
			const ImmutableVector<int> & Counts() const;
		};

		// Construction
		template <class TYPE> inline
		SparseMatrix<TYPE>::SparseMatrix()
		{
		}

		template <class TYPE>
		SparseMatrix<TYPE>::SparseMatrix(Vector<TYPE> values, Vector<int> indices, Vector<int> counts)
			: values(values), indices(indices), counts(counts)
		{
#ifdef _DEBUG
			const int ncols = counts.Size() - 1;
			_ASSERT(values.Size() == counts(ncols), Utilities::Exceptions::Exception("SparseMatrix: Invalid values size"));
			_ASSERT(indices.Size() == counts(ncols), Utilities::Exceptions::Exception("SparseMatrix: Invalid indices size"));
			_ASSERT(Internal::LDL_valid_matrix(ncols,
					Cast<const Array<int> >(counts),
					Cast<const Array<int> >(indices)) == 1,
					Utilities::Exceptions::Exception("SparseMatrix: invalid matrix"));
#endif
		}

		// Properties
		template <class TYPE> inline
		int SparseMatrix<TYPE>::NumberOfColumns() const
		{
			return this->counts.Size() - 1;
		}

		template <class TYPE> inline
		TYPE SparseMatrix<TYPE>::Value(int index) const
		{
			return this->values(index);
		}

		template <class TYPE> inline
		int SparseMatrix<TYPE>::Index(int index) const
		{
			return this->indices(index);
		}

		template <class TYPE> inline
		int SparseMatrix<TYPE>::Count(int col) const
		{
			return this->counts(col);
		}

		template <class TYPE> inline
		const ImmutableVector<TYPE> & SparseMatrix<TYPE>::Values() const
		{
			return this->values;
		}

		template <class TYPE> inline
		const ImmutableVector<int> & SparseMatrix<TYPE>::Indices() const
		{
			return this->indices;
		}

		template <class TYPE> inline
		const ImmutableVector<int> & SparseMatrix<TYPE>::Counts() const
		{
			return this->counts;
		}
	}
}

#endif
