ValidateSparseMatrix <- function(x)
{
	ValidateDouble(x$values)
	ValidateInteger(x$indices)
	ValidateInteger(x$counts)
}

# Generic function
glmmGS.SparseMatrix <- function(object, ...)
{
	UseMethod("glmmGS.SparseMatrix")
}

# Construct sparse matrix from vector
glmmGS.SparseMatrix.numeric <- function(object, ...)
{
	values <- object
	list <- list(...)
	indices <- list[[1L]]
	counts <- list[[2L]]
	
	# Validate arguments
	ValidateDouble(values)
	ValidateInteger(indices)
	ValidateInteger(counts)
	
	# Check counts
	if (length(counts) == 0L)
		stop("\'counts\' must have length greater than zero")
	if (counts[1L] != 0L)
		stop("\'counts[1]\' must be equal to zero")
	ncols <- length(counts) - 1L
	for (j in 1L:ncols)
	{
		if (counts[j + 1L] < counts[j])
			stop("\'counts\' must be monotonic non-descending")
	}

	# Retrieve total number of non-zero entries
	nz <- counts[ncols + 1L]
	
	# Check length of values and indices
	if (length(values) != nz)
		stop("Wrong length of \'values\'")
	if (length(indices) != nz)
		stop("Wrong length of \'indices\'")
	
	# Check indices
	for (j in 1L:ncols)
	{
		for (p in counts[j]:(counts[j + 1L] - 1L))
		{
			# Notice that p is zero-based
			if ((indices[p + 1L] < 0L) || (indices[p + 1L] >= ncols))
				stop("\'indices\' values must be in [0, ncols - 1]")
		}
	}
	
	# Create object
	sparse.matrix <- list()
	class(sparse.matrix) <- "glmmGS.SparseMatrix"
	sparse.matrix$values <- values
	sparse.matrix$indices <- indices
	sparse.matrix$counts <- counts
	sparse.matrix
}

# Construct sparse matrix
glmmGS.SparseMatrix.matrix <- function(object, ...)
{
	matrix <- object
	
	# Validate argument
	if (is.null(matrix) || !is.matrix(matrix))
		stop("Invalid input")
	
	# Build sparse matrix
	# 1) Count non-zero elements
	ncols <- ncol(matrix)
	counts <- integer(ncols + 1L)
	count.total <- 0L
	counts[1L] <- 0L
	for (j in 1L:ncols)
	{
		nz <- which(matrix[, j] != 0)
		count.total <- count.total + length(nz)
		counts[j + 1L] <- count.total
	}
	
	# 2) Set values and indices
	values <- double(count.total)
	indices <- integer(count.total)
	index <- 0L
	for (j in 1L:ncols)
	{
		nz <- which(matrix[, j] != 0)
		lnz <- length(nz)
		if (lnz > 0L)
		{
			values[index + 1L:lnz] <- matrix[nz, j]
			indices[index + 1L:lnz] <- nz - 1L # must be zero-based
			index = index + lnz
		}
	}
	
	# Create sparse matrix
	glmmGS.SparseMatrix(values, indices, counts)
}
