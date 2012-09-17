# Parsing utility funtions
GetVariables <- function(token)
{
	token <- unlist(strsplit(token, "\\~"))[1L]
	token <- unlist(strsplit(token, "\\|"))[1L]
	unlist(strsplit(token, "\\+"))
}

GetFactor <- function(token)
{
	token <- unlist(strsplit(token, "\\~"))[1L]
	unlist(strsplit(token, "\\|"))[2L]
}

GetCovarianceModel <- function(token)
{
	unlist(strsplit(token, "\\~"))[2L]
}

# Getter utility functions
GetNumberOfVariables <- function(block)
{
	nvars <- 0L
	for (var in block$covariates)
		nvars <- nvars + ifelse(is.matrix(var$value), ncol(var$value), 1L)
	nvars
}

GetNumberOfLevels <- function(block)
{
	ifelse(attr(block, "type") == "stratified", nlevels(block$factor$value), 0L)
}

GetNumberOfVarianceComponents <- function(block)
{
	size <- 0L
	if (attr(block, "effects") == "random")
	{
		covariance.model <- block$covariance.model$value
		
		if (attr(block, "type") == "dense")
		{
			if (class(covariance.model) %in% c(
				"glmmGS.IdentityCovarianceModel",
				"glmmGS.PrecisionModel"))
			{
				size <- 1L
			}
			
		}
		else if (attr(block, "type") == "stratified")
		{
			if (class(covariance.model) %in% c(
					"glmmGS.IdentityCovarianceModel",
					"glmmGS.PrecisionModel",
					"glmmGS.SparsePrecisionModel"))
			{
				size <- GetNumberOfVariables(block)
			}
		}
	}
}

# glmmGS.Block - S3 class constructor 
glmmGS.Block <- function(token, data, covariance.models)
{
#	print("Begin block")
#	print(gc())
	
	variable.names <- GetVariables(token)
	factor.name <- GetFactor(token)
	covariance.model.name <- GetCovarianceModel(token)
	
	# Initialize block
	block <- new.env(hash = FALSE, parent = emptyenv())
	class(block) <- "glmmGS.Block"
	block$name <- token
	
	# Set covariates
	block$covariates <- list()
	for (i in 1L:length(variable.names))
	{
		varname <- variable.names[i]
		# Avoid deep copies
		block$covariates[[i]] <- new.env(hash = FALSE, parent = emptyenv())
		block$covariates[[i]]$name <- varname
		if (varname != "1")
		{
			value <- get(varname, data)
			block$covariates[[i]]$value <- value
		}
	}
	
	# Set factor
	if (!is.na(factor.name))
	{
		# Avoid deep copies
		block$factor <- new.env(hash = FALSE, parent = emptyenv())
		block$factor$name <- factor.name
		value <- factor(get(factor.name, data))
		block$factor$value <- value
		block$factor$indices <- as.integer(value) - 1L 
	}
	
	# Set covariance model
	if (!is.na(covariance.model.name))
	{
		# Avoid deep copies
		block$covariance.model <- list()
		block$covariance.model$name <- covariance.model.name
		value <- get(covariance.model.name, covariance.models)
		block$covariance.model$value <- value
	}
	
	# Add attributes
	if (is.null(block$covariance.model))
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
	
#	print(gc())
#	print("End block")
	
	block
}
