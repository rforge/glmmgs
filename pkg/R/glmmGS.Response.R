# glmmGS.Response
glmmGS.Response <- function(formula, family)
{
	if (class(formula) != "formula" || class(family) != "family")
		stop("Invalid argument")
	
	# Initialize response object
	response <- list()
	class(response) <- "glmmGS.Response"
	
	# Get response string
	string <- as.character(formula)[2L]
	
	# Removed blanks
	string <- gsub(" ", "", string)
	
	# Split response string into tokens  
	token <- unlist(strsplit(string, "\\("))
	token <- unlist(strsplit(token, "\\)"))
	
	# Check number of tokens
	if (length(token) != 1L)
		stop("Invalid response formula")

	# Get response
	if (family$family == "binomial")
	{
		# Get varnames
		varnames <- unlist(strsplit(token, "/"))
		response$name <- varnames
		if (length(varnames) == 1L)
		{
			response$family <- "bernoulli"
		}
		else if (length(varnames) == 2L)
		{
			response$family <- "binomial"
		}
		else
		{
			stop("Invalid response formula")
		}
	}
	else
	{
		response$name <- token
		response$family <- family$family
	}
	
	response
}
