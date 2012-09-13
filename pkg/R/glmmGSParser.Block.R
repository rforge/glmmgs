# glmmGSParser.Block - S3 class constructor 
glmmGSParser.Block <- function(vars, factor, cov.model)
{
	# Validate arguments
	if (missing(vars) || is.null(vars) || is.na(vars) || !is.character(vars))
	{
		stop("\'vars\' must be a non missing character vector")
	}
	if (missing(factor) || is.null(factor) || is.na(factor))
	{
		factor <- NULL
	}
	else if (!is.character(factor))
	{
		stop("\'factor\' must be a character vector")
	}
		
	if (missing(cov.model) || is.null(cov.model) || is.na(cov.model))
	{
		cov.model <- NULL
	}
	else if (!is.character(cov.model))
	{
		stop("\'cov.model\' must be a character vector")
	}
	
	# Set list
	block <- list(vars = vars, factor = factor, 	cov.model = cov.model)
	
	# Add attributes
	class(block) <- "glmmGSParser.Block"
	if (is.null(block$cov.model))
	{
		attr(block, "effects") <- "fixed"
	}
	else
	{
		attr(block, "effects") <- "random"
	}
	if (is.null(block$factor))
	{
		attr(block, "type") <- "dense"
	}
	else
	{
		attr(block, "type") <- "stratified"
	}		
	
	block
}

