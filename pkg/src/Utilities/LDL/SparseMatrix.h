#pragma once

#include "../NewTypes/Array.h"

namespace Utilities
{
	namespace LDL
	{
		using namespace Utilities::NewTypes;

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
			SparseMatrix(const SparseMatrix<TYPE> & src);
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

		template <class TYPE> inline
		SparseMatrix<TYPE>::SparseMatrix(const SparseMatrix<TYPE> & src)
			: ncols(src.ncols), values(src.values), indices(src.indices), counts(src.counts)
		{
		}

		template <class TYPE> inline
		SparseMatrix<TYPE>::SparseMatrix(int ncols, NewTypes::Vector<TYPE> values, NewTypes::Vector<int> indices, NewTypes::Vector<int> counts)
			: ncols(ncols), values(values), indices(indices), counts(counts)
		{
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
