#pragma once

#include "../NewTypes/NewTypes.h"
#include "Internal/LDL.h"

namespace Utilities
{
	namespace LDL
	{
		using namespace NewTypes;

		// Data structure for SparseCholeskyDecomposition
		template <class TYPE>
		class SparseMatrix
		{
		private:
			// Fields
			int ncols;
			NewTypes::Vector<TYPE> values;
			NewTypes::Vector<int> indices;
			NewTypes::Vector<int> counts;

		public:
			// Construction
			SparseMatrix();
			SparseMatrix(int ncols, NewTypes::Vector<TYPE> values, NewTypes::Vector<int> indices, NewTypes::Vector<int> counts);

			// Properties
			int NumberOfColumns() const;
			TYPE Value(int index) const;
			int Index(int index) const;
			int Count(int col) const;
			const NewTypes::Vector<TYPE> Values() const;
			const NewTypes::Vector<int> Indices() const;
			const NewTypes::Vector<int> Counts() const;
		};

		// Construction
		template <class TYPE> inline
		SparseMatrix<TYPE>::SparseMatrix()
			: ncols(0)
		{
		}

		template <class TYPE>
		SparseMatrix<TYPE>::SparseMatrix(int ncols, NewTypes::Vector<TYPE> values, NewTypes::Vector<int> indices, NewTypes::Vector<int> counts)
			: ncols(ncols), values(values), indices(indices), counts(counts)
		{
#ifdef _DEBUG
			_ASSERT(counts.Size() == ncols + 1, Utilities::Exceptions::Exception("SparseMatrix: Invalid count size"));
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
			return this->ncols;
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
		const NewTypes::Vector<TYPE> SparseMatrix<TYPE>::Values() const
		{
			return this->values;
		}

		template <class TYPE> inline
		const NewTypes::Vector<int> SparseMatrix<TYPE>::Indices() const
		{
			return this->indices;
		}

		template <class TYPE> inline
		const NewTypes::Vector<int> SparseMatrix<TYPE>::Counts() const
		{
			return this->counts;
		}
	}
}
