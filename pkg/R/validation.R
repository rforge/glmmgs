ValidateLength <- function(x, length)
{
	if (!is.null(length) && length(x) != length)
	{
		stop("Invalid vector length")
	}
}

ValidateInteger <- function(x, length = NULL)
{
	if (class(x) != "integer")
	{
		stop("Invalid type")
	}
	ValidateLength(x, length)
}

ValidateDouble <- function(x, length = NULL)
{
	if (class(x) != "numeric")
	{
		stop("Invalid type")
	}
	ValidateLength(x, length)
}

ValidateCharacter <- function(x, length = NULL)
{
	if (class(x) != "character")
	{
		stop("Invalid type")
	}
	ValidateLength(x, length)
}

ValidateDimension <- function(x, dim)
{
	xdim <- dim(x)
	if (!is.null(dim) && (xdim[1L] != dim[1L] || xdim[2L] != dim[2L]))
	{
		stop("Invalid matrix dimension")
	}
}

ValidateMatrixDouble <- function(x, dim = NULL)
{
	if (class(x) != "matrix")
	{
		stop("Invalid type")
	}
	ValidateDimension(x, dim)
}
