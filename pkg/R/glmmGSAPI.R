# R low-level interface with GlmmAPI functions

# Error buffer
error.buffer = sprintf("%1024s", "")

# Get error occurred in last API call
glmmGSAPI.GetLastError <- function()
{

	# Get error message
	size = nchar(error.buffer[1])
	out = .C("GlmmGSRAPI_GetLastError", error.buffer[1], size, PACKAGE = "glmmGS")
	msg = out[[1]]
	
	# Trim trailing spaces
	#msg = sub(" +$", "", out[[1]])
	
	# Check error
	if (msg != "")
		stop(msg)
}

# Clean-up glmmGSAPI
glmmGSAPI.Tidy <- function()
{
	.C("GlmmGSRAPI_Tidy", PACKAGE = "glmmGS")
}

# Begin model definition
glmmGSAPI.BeginModel <- function()
{
	.C("GlmmGSRAPI_BeginModel", PACKAGE = "glmmGS")
	glmmGSAPI.GetLastError()
}

# End model definition
glmmGSAPI.EndModel <- function()
{
	.C("GlmmGSRAPI_EndModel", PACKAGE = "glmmGS")
	glmmGSAPI.GetLastError()
}

# Begin response
glmmGSAPI.BeginResponse <- function(family)
{
	if (is.character(family) == FALSE)
		stop("Invalid type")
	if (length(family) != 1L)
		stop("Invalid size")
	size = as.integer(nchar(family[1]))
	.C("GlmmGSRAPI_BeginResponse", family[1], size, PACKAGE = "glmmGS")
	glmmGSAPI.GetLastError()
}

# End response
glmmGSAPI.EndResponse <- function()
{
	.C("GlmmGSRAPI_EndResponse", PACKAGE = "glmmGS")
	glmmGSAPI.GetLastError()
}

# Begin fixed-effects
glmmGSAPI.BeginFixedEffects <- function()
{
	.C("GlmmGSRAPI_BeginFixedEffects", PACKAGE = "glmmGS")
	glmmGSAPI.GetLastError()
}

# End fixed-effects
glmmGSAPI.EndFixedEffects <- function()
{
	.C("GlmmGSRAPI_EndFixedEffects", PACKAGE = "glmmGS")
	glmmGSAPI.GetLastError()
}

# Begin random-effects
glmmGSAPI.BeginRandomEffects <- function()
{
	.C("GlmmGSRAPI_BeginRandomEffects", PACKAGE = "glmmGS")
	glmmGSAPI.GetLastError()
}

# End random-effects
glmmGSAPI.EndRandomEffects <- function()
{
	.C("GlmmGSRAPI_EndRandomEffects", PACKAGE = "glmmGS")
	glmmGSAPI.GetLastError()
}

# Begin global block
glmmGSAPI.BeginGlobalBlock <- function()
{
	.C("GlmmGSRAPI_BeginGlobalBlock", PACKAGE = "glmmGS")
	glmmGSAPI.GetLastError()
}

# End global block
glmmGSAPI.EndGlobalBlock <- function()
{
	.C("GlmmGSRAPI_EndGlobalBlock", PACKAGE = "glmmGS")
	glmmGSAPI.GetLastError()
}

# Begin stratified block
glmmGSAPI.BeginStratifiedBlock <- function(levels)
{
	size = as.integer(length(levels))
	if (is.integer(levels))
	{
		.C("GlmmGSRAPI_BeginStratifiedBlock", levels, size, DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
		glmmGSAPI.GetLastError()
	}
	else
	{
		stop("Invalid factor type")
	}
}

# End stratified block
glmmGSAPI.EndStratifiedBlock <- function()
{
	.C("GlmmGSRAPI_EndStratifiedBlock", PACKAGE = "glmmGS")
	glmmGSAPI.GetLastError()
}

# Add response vector
glmmGSAPI.AddResponse <- function(values)
{
	size <- as.integer(length(values))
	if (is.integer(values))
	{
		.C("GlmmGSRAPI_AddResponseInt", values, size, DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
		glmmGSAPI.GetLastError()
	}
	else
	{
		stop("Invalid type")
	}
}

# Add vector of counts (for binomial response)
glmmGSAPI.AddCounts <- function(values)
{
	size <- as.integer(length(values))
	if (is.integer(values))
	{
		.C("GlmmGSRAPI_AddCountsInt", values, size, DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
		glmmGSAPI.GetLastError()
	}
	else
	{
		stop("Invalid type")
	}
}

# Add offset
glmmGSAPI.AddOffset <- function(values)
{
	size <- as.integer(length(values))
	if (is.integer(values))
	{
		.C("GlmmGSRAPI_AddOffsetInt", values, size, DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
		glmmGSAPI.GetLastError()
	}
	else if (is.double(values))
	{
		.C("GlmmGSRAPI_AddOffsetDbl", values, size, DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
		glmmGSAPI.GetLastError()
	}
	else
	{
		stop("Invalid type")
	}
}

# Add intercept
glmmGSAPI.AddIntercept <- function()
{
	.C("GlmmGSRAPI_AddIntercept", PACKAGE = "glmmGS")
	glmmGSAPI.GetLastError()
}

# Add a covariate
glmmGSAPI.AddCovariate <- function(values)
{
	if (is.vector(values))
	{
		size <- as.integer(length(values))
		if (is.integer(values))
		{
			.C("GlmmGSRAPI_AddCovariateInt", values, size, DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
			glmmGSAPI.GetLastError()
		}
		else if (is.double(values))
		{
			.C("GlmmGSRAPI_AddCovariateDbl", values, size, DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
			glmmGSAPI.GetLastError()
		}
		else
		{
			stop("Invalid type")
		}
	}
	else if (is.matrix(values))
	{
		dimensions <- dim(values)
		if (is.integer(values))
		{
			.C("GlmmGSRAPI_AddCovariatesInt", values, dimensions, DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
			glmmGSAPI.GetLastError()
		}
		else if (is.double(values))
		{
			.C("GlmmGSRAPI_AddCovariatesDbl", values, dimensions, DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
			glmmGSAPI.GetLastError()
		}
		else
		{
			stop("Invalid type")
		}
	}
	else
	{
		stop("Invalid type")
	}
}

# Add identity covariance model
glmmGSAPI.AddIdentityCovarianceModel <- function()
{
	.C("GlmmGSRAPI_AddIdentityCovarianceModel", PACKAGE = "glmmGS")
	glmmGSAPI.GetLastError()
}

# Add precision model
glmmGSAPI.AddPrecisionModel <- function(R)
{
	if (is.matrix(R) && is.double(R))
	{
		dim <- dim(R)
		.C("GlmmGSRAPI_AddPrecisionModel", R, dim[1], dim[2], DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
		glmmGSAPI.GetLastError()
	}
	else
	{
		stop("Invalid type")
	}
}

# Add sparse-precision model
glmmGSAPI.AddSparsePrecisionModel <- function(R)
{
	if (class(R) == "glmmGS.SparseMatrix")
	{
		ncols = as.integer(length(R$counts)) - 1L
		.C("GlmmGSRAPI_AddSparsePrecisionModel", R$values, R$indices, R$counts, ncols, DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
		glmmGSAPI.GetLastError()
	}
	else
	{
		stop("Invalid type")
	}
}

# Fit current model
glmmGSAPI.Fit <- function(relative.tolerance, absolute.tolerance, maxiter, verbose)
{
	.C("GlmmGSRAPI_Fit", as.double(relative.tolerance), as.double(absolute.tolerance), 
			as.integer(maxiter), as.integer(verbose), PACKAGE = "glmmGS")
	glmmGSAPI.GetLastError()
}

# Get estimated fixed-effects coefficients
glmmGSAPI.GetFixef <- function(fixef)
{
	for (i in 1:length(fixef))
	{
		block <- fixef[[i]]$block
		if (class(block) != "glmmGS.Block" || attr(block, "effects") != "fixed") 
			stop("Invalid argument")
		
		if (attr(block, "type") == "dense")
		{
			index <- i - 1L
			nvars <- .GetNumberOfVariables(block)
			beta <- double(nvars)
			beta.cov <- double(nvars * nvars)

			.C("GlmmGSRAPI_GetFixefDenseBlock", 
					index,
					beta,
					beta.cov,
					nvars,
					DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
			
			fixef[[i]]$beta <- beta
			fixef[[i]]$beta.cov <- beta.cov
		}
		else if (attr(block, "type") == "stratified")
		{
			index <- i - 1L
			nvars <- .GetNumberOfVariables(block)
			nlevels <- .GetNumberOfLevels(block)
			beta <- double(nvars * nlevels)
			beta.cov <- double(nvars * nvars * nlevels)
			
			.C("GlmmGSRAPI_GetFixefStratifiedBlock", 
					index,
					beta,
					beta.cov,
					nvars,
					nlevels,
					DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
			
			fixef[[i]]$beta <- beta
			fixef[[i]]$beta.cov <- beta.cov
		}
	}
	
	fixef
}

glmmGSAPI.GetRanef <- function(ranef, covariance.models)
{
	ranef
}

# Get number of iterations
glmmGSAPI.GetIterations <- function()
{
	iterations <- integer(1L)
	.C("GlmmGSRAPI_GetIterations", iterations, DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
	glmmGSAPI.GetLastError()
	
	iterations
}
