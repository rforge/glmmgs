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
		nvars <- nvars + var$dim
	nvars
}

GetNumberOfLevels <- function(block)
{
	ifelse(attr(block, "type") == "stratified", length(block$factor$levels), 0L)
}

GetNumberOfVarianceComponents <- function(block)
{
	size <- 0L
	if (attr(block, "effects") == "random")
	{
		covariance.model <- block$covariance.model
		
		if (attr(block, "type") == "dense")
		{
			if (covariance.model$class %in% c(
				"glmmGS.IdentityModel",
				"glmmGS.PrecisionModel"))
			{
				size <- 1L
			}
			
		}
		else if (attr(block, "type") == "stratified")
		{
			if (covariance.model$class %in% c(
					"glmmGS.MultivariateIdentityModel"))
			{
				size <- GetNumberOfVariables(block)
				size <- as.integer((size * (size + 1)) / 2)
			}
			else if (covariance.model$class %in% c(
					"glmmGS.IdentityModel",
					"glmmGS.PrecisionModel",
					"glmmGS.SparsePrecisionModel"
							))
			{
				size <- GetNumberOfVariables(block)
			}
		}
	}
	
	size
}

# glmmGS.Block - S3 class constructor 
glmmGS.Block <- function(token, data, covariance.models)
{
	variable.names <- GetVariables(token)
	factor.name <- GetFactor(token)
	covariance.model.name <- GetCovarianceModel(token)
	
	# Check for duplicate variables
	if (sum(duplicated(variable.names)) > 0L)
		stop(paste("Duplicate variables not allowed in same GS block", token, sep = ": "))
	
	# If present, set intercept as first variable
	i <- which(variable.names == "1")
	if (length(i) > 0L && i > 1L)
	{
		# Rotate variable names
		i0 <- (1L:i)
		i1 <- (i0 %% i) + 1L
		variable.names[i1] <- variable.names[i0] 
	}
	
	# Initialize block
	block <- list()
	class(block) <- "glmmGS.Block"
	block$name <- token
	
	# Set covariates
	block$covariates <- list()
	for (i in 1L:length(variable.names))
	{
		varname <- variable.names[i]
		block$covariates[[i]] <- list()
		block$covariates[[i]]$name <- varname
		if (varname != "1")
		{
			var <- get(varname, data)
			block$covariates[[i]]$class <- class(var)
			if (is.matrix(var))
			{
				block$covariates[[i]]$dim <- ncol(var)
			}
			else
			{
				block$covariates[[i]]$dim <- 1L
			}
		}
		else
		{
			block$covariates[[i]]$class <- "intercept"
			block$covariates[[i]]$dim <- 1L
		}
	}
	
	# Set factor
	if (!is.na(factor.name))
	{
		g <- factor(get(factor.name, data))
		block$factor <- list()
		block$factor$name <- factor.name
		block$factor$levels <- levels(g) 
		block$factor$indices <- as.integer(g) - 1L 
	}
	
	# Set covariance model
	if (!is.na(covariance.model.name))
	{
		cov.model <- get(covariance.model.name, covariance.models)
		block$covariance.model <- list()
		block$covariance.model$name <- covariance.model.name
		block$covariance.model$class <- class(cov.model)
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
	
	block
}
