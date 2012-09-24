# glmmGS.Response
glmmGS.Response <- function(formula, family, data)
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
	tokens <- unlist(strsplit(string, "\\("))
	tokens <- unlist(strsplit(tokens, "\\)"))
	
	# Check number of tokens
	if (length(tokens) != 1L)
		stop("Invalid response formula")

	# Get response
	if (family$family == "binomial")
	{
		# Get varnames
		varnames <- unlist(strsplit(tokens, "/"))
		
		# Set name and value
		if (length(varnames) == 0L)
			stop("Invalid response formula")
		response$name <- varnames[1L]
		value <- get(varnames[1L], data)
		response$value <- value
		
		# Set size
		if (length(varnames) == 1L)
		{
			size <- rep(1L, length(value))
			response$size <- size 
		}
		else if (length(varnames) == 2L)
		{
			size <- get(varnames[2L], data)
			response$size <- size
		}
		else
		{
			stop("Invalid response formula")
		}
	}
	else
	{
		varname <- tokens
		
		response$name <- varname
		value <- get(varname, data)
		response$value <- value
	}
	
	response
}
