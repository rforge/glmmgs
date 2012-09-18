#ifndef UTILITIES_LDL_SPARSEMATRIXFUNCTIONS_H
#define UTILITIES_LDL_SPARSEMATRIXFUNCTIONS_H

#include "../Utilities.h"
#include "SparseMatrix.h"

namespace Utilities 
{
	namespace LDL
	{
		Vector<double> ColumnSums(const SparseMatrix<double> & A);
	}
}

#endif
