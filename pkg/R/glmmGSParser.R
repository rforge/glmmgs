# glmmGS parser utility funtions

glmmGSParser.GetVariables <- function(token)
{
	token <- unlist(strsplit(token, "\\~"))[1]
	token <- unlist(strsplit(token, "\\|"))[1]
	unlist(strsplit(token, "\\+"))
}

glmmGSParser.GetFactor <- function(token)
{
	token <- unlist(strsplit(token, "\\~"))[1]
	unlist(strsplit(token, "\\|"))[2]
}

glmmGSParser.GetCovModel <- function(token)
{
	unlist(strsplit(token, "\\~"))[2]
}

glmmGSParser.GetBlocks <- function(tokens)
{
	n = length(tokens)
	blocks <- list()
	for (i in 1:n)
	{
		token <- tokens[i]
		blocks[[i]] <- glmmGSParser.Block(
				vars = glmmGSParser.GetVariables(token), 
				factor = glmmGSParser.GetFactor(token), 
				cov.model = glmmGSParser.GetCovModel(token))
	}
	blocks
}

glmmGSParser.GetResponse <- function(formula, family)
{
	# Get predictor string
	response <- as.character(formula)[2]
	vars <- NULL
	if (family == "binomial")
	{
		# Remove parenthesis, if any
		response <- sub("\\(", "", response)
		response <- sub("\\)", "", response)
		vars <- unlist(strsplit(response, "/"))
	}
	else
	{
		vars <- response
	}
	list(family = family, vars = vars)
}

glmmGSParser.GetPredictors <- function(formula)
{
	# Get predictor string
	predictors <- as.character(formula)[3]
	
	# Removed blanks
	predictors <- gsub(" ", "", predictors)
	
	# Put parentheses around offset
	predictors <- sub("offset\\(", "\\(offset\\)\\(", predictors)
	
	# Split predictor string into tokens  
	tokens <- unlist(strsplit(predictors, "\\("))
	tokens <- unlist(strsplit(tokens, "\\)"))
	
	# Get offset, if any
	offset <- NULL
	i <- match("offset", tokens)
	if (!is.na(i) && i < length(tokens) && tokens[i + 1L] != "+")
	{
		# Set offset
		offset <- tokens[i + 1L]
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
	}

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
	blocks <- glmmGSParser.GetBlocks(tokens)
	
	# Set fixed and random effect blocks
	fixef <- list()
	ranef <- list()
	for (block in blocks)
	{
		# Assign fixef or ranef blocks
		if (attr(block, "effects") == "fixed")
		{
			block$cov.model <- NULL
			fixef[[length(fixef) + 1L]] <- block
		}
		else if (attr(block, "effects") == "random")
		{
			ranef[[length(ranef) + 1L]] <- block
		}
		else
		{
			stop("Invalid \'effects\' attribute")
		}
	}
	
	list(offset = offset, fixef = fixef, ranef = ranef)
}

