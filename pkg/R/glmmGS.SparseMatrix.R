# Internal function to construct sparse matrix
glmmGS.CreateSparseMatrix = function(values, indices, counts)
{
	# Check types
	if ((is.integer(counts) == FALSE) || (is.vector(counts) == FALSE))
	{
		stop("\'counts\' must be a vector of integers");
	}
	if ((is.double(values) == FALSE) || (is.vector(values) == FALSE))
	{
		stop("\'values\' must be a vector of double");
	}
	if ((is.integer(indices) == FALSE) || (is.vector(indices) == FALSE))
	{
		stop("\'indices\' must be a vector of integers");
	}
	
	# Check length of counts
	if (length(counts) == 0L)
	{
		stop("\'counts\' must have length greater than zero");
	}
	
	# Set number of columns
	ncols = length(counts) - 1L;
	
	# Check values of counts
	if (counts[1] != 0L)
	{
		stop("\'counts[1]\' must be equal to zero");
	}
	for (j in 1:ncols)
	{
		if (counts[j + 1L] < counts[j])
		{
			stop("\'counts\' must be monotonic non-descending");
		}
	}

	# Retrieve total number of non-zero entries
	nz = counts[ncols + 1L];
	
	# Check length of values and indices
	if (length(values) != nz)
	{
		stop("Wrong length of \'values\'");
	}
	if (length(indices) != nz)
	{
		stop("Wrong length of \'indices\'");
	}
	
	# Check indices
	for (j in 1:ncols)
	{
		for (p in counts[j]:(counts[j + 1L] - 1L))
		{
			# Notice that p is zero-based
			if ((indices[p + 1L] < 0L) || (indices[p + 1L] >= ncols))
			{
				stop("\'indices\' values must be in [0, ncols - 1]")
			}
		}
	}
	
	# Set return list
	retval = list(values = values, indices = indices, counts = counts);
	
	# Attach "sparse" attribute to list
	attr(retval, "sparse.matrix") = TRUE;
	
	# Return
	return(retval);
}

# Construct sparse matrix
glmmGS.SparseMatrix = function(...)
{
	# Get argument list
	ls = list(...);
	
	# Initialize return value
	retval = NULL;
	
	# Parse argument list
	if (length(ls) == 1L)
	{
		R = ls[[1]];
		if ((is.null(R) == TRUE) || (is.matrix(R) == FALSE) || (nrow(R) != ncol(R)))
		{
			stop("Invalid matrix");
		}
		else
		{
			# Build sparse matrix
			# 1) Count non-zero elements
			ncols = ncol(R);
			counts = integer(ncols + 1L);
			count.total = 0L;
			counts[1] = 0L;
			for (j in 1:ncols)
			{
				nz = which(R[, j] != 0);
				count.total = count.total + length(nz);
				counts[j + 1L] = count.total;
			}
			
			# 2) Set values and indices
			values = double(count.total);
			indices = integer(count.total);
			index = 0L;
			for (j in 1:ncols)
			{
				nz = which(R[, j] != 0);
				lnz = length(nz);
				values[index + 1:lnz] = R[nz, j];
				indices[index + 1:lnz] = nz - 1L; # must be zero-based
				index = index + lnz;
			}
			
			# Create sparse matrix
			retval = glmmGS.CreateSparseMatrix(values, indices, counts);
		}
	}
	else if (length(ls) == 3L)
	{
		# Retrieve data
		values = ls[[1]];
		indices = ls[[2]];
		counts = ls[[3]];
		
		# Create sparse matrix
		retval = glmmGS.CreateSparseMatrix(values, indices, counts);
	}
	else
	{
		stop("Invalid argument list");
	}
	
	return(retval);
}
