# R low-level interface with GlmmAPI functions

# Error buffer
error.buffer = sprintf("%1024s", "")

# Get error occurred in last API call
glmmGSAPI.GetLastError <- function()
{

	# Get error message
	size = nchar(error.buffer[1L])
	out = .C("GlmmGSRAPI_GetLastError", error.buffer[1L], size, PACKAGE = "glmmGS")
	msg = out[[1L]]
	
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
	ValidateCharacter(family, 1L)
	size = nchar(family)
	.C("GlmmGSRAPI_BeginResponse", family, size, PACKAGE = "glmmGS")
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
		stop("Invalid argument")
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
	size <- length(values)
	if (class(values) == "integer")
	{
		.C("GlmmGSRAPI_AddResponseInt", values, size, DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
		glmmGSAPI.GetLastError()
	}
	else if (class(values) == "numeric")
	{
		.C("GlmmGSRAPI_AddResponseDbl", values, size, DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
		glmmGSAPI.GetLastError()
	}
	else
	{
		stop("Invalid argument")
	}
}

# Add vector of counts (for binomial response)
glmmGSAPI.AddCounts <- function(values)
{
	size <- length(values)
	if (class(values) == "integer")
	{
		.C("GlmmGSRAPI_AddCountsInt", values, size, DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
		glmmGSAPI.GetLastError()
	}
	else
	{
		stop("Invalid argument")
	}
}

# Add offset
glmmGSAPI.AddOffset <- function(values)
{
	size <- length(values)
	if (class(values) == "integer")
	{
		.C("GlmmGSRAPI_AddOffsetInt", values, size, DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
		glmmGSAPI.GetLastError()
	}
	else if (class(values) == "numeric")
	{
		.C("GlmmGSRAPI_AddOffsetDbl", values, size, DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
		glmmGSAPI.GetLastError()
	}
	else
	{
		stop("Invalid argument")
	}
}

# Add intercept
glmmGSAPI.AddIntercept <- function(duplicate)
{
	ValidateInteger(duplicate, 1L)
	
	.C("GlmmGSRAPI_AddIntercept", duplicate, PACKAGE = "glmmGS")
	glmmGSAPI.GetLastError()
}

# Add a covariate
glmmGSAPI.AddCovariate <- function(values, duplicate)
{
	ValidateInteger(duplicate, 1L)
	
	if (is.vector(values))
	{
		size <- as.integer(length(values))
		if (is.integer(values))
		{
			.C("GlmmGSRAPI_AddCovariateInt", values, size, duplicate, DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
			glmmGSAPI.GetLastError()
		}
		else if (is.double(values))
		{
			.C("GlmmGSRAPI_AddCovariateDbl", values, size, duplicate, DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
			glmmGSAPI.GetLastError()
		}
		else
		{
			stop("Invalid argument")
		}
	}
	else if (is.matrix(values))
	{
		dim <- dim(values)
		if (is.integer(values))
		{
			.C("GlmmGSRAPI_AddCovariatesInt", values, dim, duplicate, DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
			glmmGSAPI.GetLastError()
		}
		else if (is.double(values))
		{
			.C("GlmmGSRAPI_AddCovariatesDbl", values, dim, duplicate, DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
			glmmGSAPI.GetLastError()
		}
		else
		{
			stop("Invalid argument")
		}
	}
	else
	{
		stop("Invalid argument")
	}
}

# Add identity covariance model
glmmGSAPI.AddIdentityCovarianceModel <- function(S)
{
	ValidateMatrixDouble(S)
	dimS <- dim(S)
	.C("GlmmGSRAPI_AddIdentityCovarianceModel", S, dimS, DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
	glmmGSAPI.GetLastError()
}

# Add precision model
glmmGSAPI.AddPrecisionModel <- function(R, S)
{
	ValidateMatrixDouble(R)
	ValidateMatrixDouble(S)
	dimR <- dim(R)
	dimS <- dim(S)
	.C("GlmmGSRAPI_AddPrecisionModel", R, dimR, S, dimS, DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
	glmmGSAPI.GetLastError()
}

# Add sparse-precision model
glmmGSAPI.AddSparsePrecisionModel <- function(R, S)
{
	ValidateSparseMatrix(R)
	ValidateMatrixDouble(S)
	
	ncols = length(R$counts) - 1L
	dimS <- dim(S)
	.C("GlmmGSRAPI_AddSparsePrecisionModel", R$values, R$indices, R$counts, ncols, S, dimS, 
			DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
	glmmGSAPI.GetLastError()
}

# Fit current model
glmmGSAPI.Fit <- function(control)
{
	ValidateControl(
			control$reltol, 
			control$abstol, 
			control$maxit, 
			control$verbose,
			control$max.update, 
			control$max.value)

	.C("GlmmGSRAPI_Fit", 
			control$reltol, 
			control$abstol, 
			control$maxit, 
			control$verbose, 
			control$max.update, 
			control$max.value, 
			PACKAGE = "glmmGS")
	glmmGSAPI.GetLastError()
}

# Get estimated fixed-effects coefficients
glmmGSAPI.GetFixef <- function(fixef)
{
	n <- length(fixef)
	if (n > 0L)
	{
		for (i in 1L:n)
		{
			block <- fixef[[i]]$block
			if (class(block) != "glmmGS.Block" || attr(block, "effects") != "fixed") 
				stop("Invalid argument")
			
			coef <- list()
			
			if (attr(block, "type") == "dense")
			{
				index <- i - 1L
				nvars <- GetNumberOfVariables(block)
				estm <- double(nvars)
				vcov <- matrix(0, nvars, nvars)
	
				.C("GlmmGSRAPI_GetFixefDenseBlock", 
						index,
						estm,
						vcov,
						nvars,
						DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
				glmmGSAPI.GetLastError()
				coef$estm <- estm
				coef$vcov <- vcov
			}
			else if (attr(block, "type") == "stratified")
			{
				index <- i - 1L
				nvars <- GetNumberOfVariables(block)
				nlevels <- GetNumberOfLevels(block)
				estm <- matrix(0, nrow = nlevels, ncol = nvars )
				vcov <- array(0, dim = c(nlevels, nvars, nvars))
				
				.C("GlmmGSRAPI_GetFixefStratifiedBlock", 
						index,
						estm,
						vcov,
						nvars,
						nlevels,
						DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
				glmmGSAPI.GetLastError()
				coef$estm <- estm
				coef$vcov <- vcov
			}
			
			fixef[[i]]$coef <- coef
		}
	}
	
	fixef
}

glmmGSAPI.GetRanef <- function(ranef)
{
	n <- length(ranef)
	
	if (n > 0L)
	{
		for (i in 1L:n)
		{
			block <- ranef[[i]]$block
			if (class(block) != "glmmGS.Block" || attr(block, "effects") != "random") 
				stop("Invalid argument")
			
			coef <- list()
			vcomp <- list()
			
			if (attr(block, "type") == "dense")
			{
				index <- i - 1L
				
				nvars <- GetNumberOfVariables(block)
				estm <- double(nvars)
				.C("GlmmGSRAPI_GetRanefDenseBlock", 
						index,
						estm,
						nvars,
						DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
				glmmGSAPI.GetLastError()
				coef$estm <- estm
				
				size <- GetNumberOfVarianceComponents(block)
				estm <- double(size)
				vcov <- matrix(0, size, size)
				.C("GlmmGSRAPI_GetVCompDenseBlock", 
						index,
						estm,
						vcov,
						size,
						DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
				glmmGSAPI.GetLastError()
				vcomp$estm <- estm
				vcomp$vcov <- vcov
			}
			else if (attr(block, "type") == "stratified")
			{
				index <- i - 1L
				
				nvars <- GetNumberOfVariables(block)
				nlevels <- GetNumberOfLevels(block)
				estm <- matrix(0, nrow = nlevels, ncol = nvars )
				.C("GlmmGSRAPI_GetRanefStratifiedBlock", 
						index,
						estm,
						nvars,
						nlevels,
						DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
				glmmGSAPI.GetLastError()
				coef$estm <- estm
				
				size <- GetNumberOfVarianceComponents(block)
				estm <- double(size)
				vcov <- matrix(0, size, size)
				.C("GlmmGSRAPI_GetVCompStratifiedBlock", 
						index,
						estm,
						vcov,
						size,
						DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
				glmmGSAPI.GetLastError()
				vcomp$estm <- estm
				vcomp$vcov <- vcov
			}
			
			ranef[[i]]$coef <- coef
			ranef[[i]]$vcomp <- vcomp
		}
	}
	
	ranef
}

# Get number of iterations
glmmGSAPI.GetIterations <- function()
{
	size <- 3L
	iterations <- integer(size)
	.C("GlmmGSRAPI_GetIterations", iterations, size, DUP = FALSE, NAOK = FALSE, PACKAGE = "glmmGS")
	glmmGSAPI.GetLastError()
	
	iterations
}
