# glmmGS.Predictors S3 class constructor
glmmGS.Predictors <- function(formula, data, covariance.models)
{
#	print("Begin predictors")
#	print(gc())
	
	# Validate arguments
	if (class(formula) != "formula")
		stop("Invalid argument")

	# Initialize response object
	predictors <- list()
	class(predictors) <- "glmmGS.Predictors"

	# Get tokens	
	# Get predictor string
	string <- as.character(formula)[3L]
	
	# Removed blanks
	string <- gsub(" ", "", string)
	
	# Put parentheses around offset
	string <- sub("offset\\(", "\\(offset\\)\\(", string)
	
	# Split predictor string into tokens  
	tokens <- unlist(strsplit(string, "\\("))
	tokens <- unlist(strsplit(tokens, "\\)"))
	
	# Get offset, if any
	i <- match("offset", tokens)
	if (!is.na(i) && i < length(tokens) && tokens[i + 1L] != "+")
	{
		# Get offset name
		offset.name <- tokens[i + 1L]
		
		# Remove offset tokens
		if (i > 1L)
		{
			tokens <- tokens[-c(i - 1L, i, i + 1L)]
		}
		else if (i < length(tokens) - 1L)
		{
			tokens <- tokens[-c(i, i + 1L, i + 2L)]
		}
		else
		{
			stop("Invalid formula")
		}
		
		# Set offset
		predictors$offset <- list()
		predictors$offset$value <- get(offset.name, data)
		predictors$offset$name <- offset.name
	}
	rm(i)

	# Remove plus signs
	len <- length(tokens)
	if (len > 1L)
	{
		# Check even tokens
		half.len <- as.integer(len / 2L)
		even <- 2L * (1:half.len)
		# Check number of tokens is odd and if even tokens are equal to "+"
		if ((2L * half.len == len) || (sum(ifelse(tokens[even] == "+", 0L, 1L)) > 0L)) 
			stop("Invalid formula", call. = FALSE)
		# Remove the "+" tokens
		tokens <- tokens[-even]
	}
	rm(len)
	
	# Set fixed and random effect blocks
	predictors$fixef <- list()
	predictors$ranef <- list()
	for (token in tokens)
	{
		block <- glmmGS.Block(token, data, covariance.models) 
		
		# Assign fixef or ranef blocks
		if (attr(block, "effects") == "fixed")
		{
			i <- length(predictors$fixef) + 1L
			predictors$fixef[[i]] <- list()
			predictors$fixef[[i]]$block <- block
		}
		else if (attr(block, "effects") == "random")
		{
			i <- length(predictors$ranef) + 1L
			predictors$ranef[[i]] <- list()
			predictors$ranef[[i]]$block <- block
		}
		else
		{
			stop("Invalid \'effects\' attribute")
		}
	}
	
#	print(gc())
#	print("End predictors")
	
	predictors
}
