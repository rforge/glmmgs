# glmmGS parser utility funtions

glmmGSParser.ParseVariables <- function(token)
{
	token <- unlist(strsplit(token, "\\~"))[1]
	token <- unlist(strsplit(token, "\\|"))[1]
	vars <- unlist(strsplit(token, "\\+"))
	vars
}

glmmGSParser.ParseFactor <- function(token)
{
	token <- unlist(strsplit(token, "\\~"))[1]
	token <- unlist(strsplit(token, "\\|"))[2]
	token
}

glmmGSParser.ParseCovModel <- function(token)
{
	token <- unlist(strsplit(token, "\\~"))[2]
	token
}

glmmGSParser.ParseBlocks <- function(tokens)
{
	n = length(tokens)
	blocks <- list()
	for (i in 1:n)
	{
		token <- tokens[i]
		vars <- glmmGSParser.ParseVariables(token)
		factor <- glmmGSParser.ParseFactor(token)
		cov.model <- glmmGSParser.ParseCovModel(token)
		blocks[[i]] <- list(vars = vars, factor = factor, cov.model = cov.model)
	}
	return(blocks)
}

glmmGSParser.GetResponse <- function(formula, family)
{
	# Get predictor string
	response <- as.character(formula[2])
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
	predictors <- as.character(formula[3])
	
	# If 'offset(', add left parenthesis
	predictors <- sub(" offset\\(", " \\(offset\\(", predictors)
	
	# Split predictor string into tokens  
	tokens <- unlist(strsplit(predictors, "\\("))
	tokens <- unlist(strsplit(tokens, "\\)"))
	
	# remove white spaces
	tokens <- gsub(" ", "", tokens)
	
	# Get offset, if any
	offset <- NULL
	i <- match("offset", tokens)
	if (!is.na(i) && i < length(tokens) && tokens[i + 1] != "+")
	{
		# Set offset
		offset <- tokens[i + 1]
		# Remove offset tokens
		tokens <- tokens[-ifelse(i + 1 < length(tokens), c(i, i + 1, i + 2), c(i, i + 1))]
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
	blocks <- glmmGSParser.ParseBlocks(tokens)
	
	# Add attributes to blocks
	for (i in 1:length(blocks))
	{
		attr(blocks[[i]], "effects") <- ifelse(is.na(blocks[[i]]$cov.model), "fixed", "random") 
		attr(blocks[[i]], "type") <- ifelse(is.na(blocks[[i]]$factor), "dense", "stratified") 
	}
	
	list(offset = offset, blocks = blocks)
}

