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
	indices <- list[[1]]
	counts <- list[[2]]
	
	# Check argument
	if (!is.double(values) || !is.vector(values))
	{
		stop("\'values\' must be a vector of doubles")
	}
	if (!is.integer(indices) || !is.vector(indices))
	{
		stop("\'indices\' must be a vector of integers")
	}
	if (!is.integer(counts) || !is.vector(counts))
	{
		stop("\'counts\' must be a vector of integers")
	}
	
	# Check length of counts
	if (length(counts) == 0L)
	{
		stop("\'counts\' must have length greater than zero")
	}
	
	# Set number of columns
	ncols <- length(counts) - 1L
	
	# Check values of counts
	if (counts[1] != 0L)
	{
		stop("\'counts[1]\' must be equal to zero")
	}
	for (j in 1:ncols)
	{
		if (counts[j + 1L] < counts[j])
		{
			stop("\'counts\' must be monotonic non-descending")
		}
	}

	# Retrieve total number of non-zero entries
	nz <- counts[ncols + 1L]
	
	# Check length of values and indices
	if (length(values) != nz)
	{
		stop("Wrong length of \'values\'")
	}
	if (length(indices) != nz)
	{
		stop("Wrong length of \'indices\'")
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
	
	# Create object
	sparse.matrix <- list(values = values, indices = indices, counts = counts)
	class(sparse.matrix) <- "glmmGS.SparseMatrix"

	# Return
	sparse.matrix
}

# Construct sparse matrix
glmmGS.SparseMatrix.matrix <- function(object, ...)
{
	matrix <- object
	
	# Validate argument
	if (is.null(matrix) || !is.matrix(matrix) || (nrow(matrix) != ncol(matrix)))
		stop("Invalid input")
	
	# Build sparse matrix
	# 1) Count non-zero elements
	ncols <- ncol(matrix)
	counts <- integer(ncols + 1L)
	count.total <- 0L
	counts[1] <- 0L
	for (j in 1:ncols)
	{
		nz <- which(matrix[, j] != 0)
		count.total <- count.total + length(nz)
		counts[j + 1L] <- count.total
	}
	
	# 2) Set values and indices
	values <- double(count.total)
	indices <- integer(count.total)
	index <- 0L
	for (j in 1:ncols)
	{
		nz <- which(matrix[, j] != 0)
		lnz <- length(nz)
		if (lnz > 0L)
		{
			values[index + 1:lnz] <- matrix[nz, j]
			indices[index + 1:lnz] <- nz - 1L # must be zero-based
			index = index + lnz
		}
	}
	
	# Create sparse matrix
	glmmGS.SparseMatrix(values, indices, counts)
}
