# Parsing utility funtions
.GetVariables <- function(token)
{
	token <- unlist(strsplit(token, "\\~"))[1]
	token <- unlist(strsplit(token, "\\|"))[1]
	unlist(strsplit(token, "\\+"))
}

.GetFactor <- function(token)
{
	token <- unlist(strsplit(token, "\\~"))[1]
	unlist(strsplit(token, "\\|"))[2]
}

.GetCovarianceModel <- function(token)
{
	unlist(strsplit(token, "\\~"))[2]
}

# glmmGS.Block - S3 class constructor 
glmmGS.Block <- function(token, data, covariance.models)
{
#	print("Begin block")
#	print(gc())
	
	variable.names <- .GetVariables(token)
	factor.name <- .GetFactor(token)
	covariance.model.name <- .GetCovarianceModel(token)
	
	# Initialize block
	block <- list()
	class(block) <- "glmmGS.Block"
	
	# Set covariates
	block$covariates <- list()
	for (i in 1:length(variable.names))
	{
		varname <- variable.names[i]
		# Avoid deep copies
		block$covariates[[i]] <- list()
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
		block$factor <- list()
		block$factor$name <- factor.name
		value <- factor(get(factor.name, data))
		block$factor$value <- value
		indices <- as.integer(value) - 1L
		block$factor$indices <- indices 
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
