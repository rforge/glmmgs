# Add response
glmmGS.AddResponse <- function(response, data)
{
	values <- get(response$name[1L], data)
	glmmGSAPI.AddResponse(values)
	if (response$family == "binomial")
	{
		counts <- get(response$name[2L], data)
		glmmGSAPI.AddCounts(counts)
	}
}

# Add offset
glmmGS.AddOffset <- function(offset, data)
{
	values <- get(offset$name, data)
	glmmGSAPI.AddOffset(values)
}

# Add variables to block
glmmGS.AddCovariates <- function(covariates, data)
{
	for (var in covariates) 
	{
		if (var$name == "1") 
		{
			glmmGSAPI.AddIntercept(var$duplicate)
		}
		else 
		{
			values <- get(var$name, data)
			glmmGSAPI.AddCovariate(values, var$duplicate)
		}
	}
}

# Add covariance model to model
glmmGS.AddCovarianceModel <- function(covariance.model, covariance.models)
{
	cov.model <- get(covariance.model$name, covariance.models)
	
	if (class(cov.model) == "glmmGS.IdentityModel") 
	{
		glmmGSAPI.AddIdentityModel(cov.model$theta)
	}
	else if (class(cov.model) == "glmmGS.MultivariateIdentityModel") 
	{
		glmmGSAPI.AddMultivariateIdentityModel(cov.model$theta)
	}
	else if (class(cov.model) == "glmmGS.PrecisionModel")
	{
		glmmGSAPI.AddPrecisionModel(cov.model$R, cov.model$theta)
	}
	else if (class(cov.model) == "glmmGS.SparsePrecisionModel") 
	{
		glmmGSAPI.AddSparsePrecisionModel(cov.model$R, cov.model$theta)
	}
	else
	{
		stop("Unsupported covariance model")
	}
}

# Main function
glmmGS <- function(formula, family, data, covariance.models, control = glmmGS.Control())
{
	# Validate and coerce formula argument
	if (missing(formula))
	{
		stop("\'formula\' argument missing with no default")
	}
	formula <- as.formula(formula)
	
	# Validate and coerce family argument
	if (missing(family)) 
	{
		stop("\'family\' argument missing with no default")
	}
	else if (is.character(family))
	{
		family <- get(family, mode = "function", envir = parent.frame())
	}
	else if (is.function(family))
	{
		family <- family()
	}
	else
	{
		stop("Invalid \'family\' argument")
	}
	
	# Vaidate family members
	if (family$family == "gaussian") 
	{
		if (family$link != "identity") 
		{
			stop("unsupported link in \'gaussian\' family")
		}
	}
	else if (family$family == "binomial") 
	{
		if (family$link != "logit") 
		{
			stop("unsupported link in \'binomial\' family")
		}
	}
	else if (family$family == "poisson") 
	{
		if (family$link != "log") 
		{
			stop("unsupported link in \'poisson\' family")
		}
	}
	else
	{
		print(family)
		stop("Unsupported family")
	}
	
	# Set data if missing
	if (missing(data))
	{
		data <- environment(formula)
	}

	# Set covariance.models if missing
	if (missing(covariance.models))
	{
		covariance.models <- environment(formula)
	}
	
	# Get response and predictor blocks
	response <- glmmGS.Response(formula, family)
	predictors <- glmmGS.Predictors(formula, data, covariance.models)
	
	# Initialize API
	glmmGSAPI.BeginModel()
	
	# Add response
	glmmGSAPI.BeginResponse(response$family)
	glmmGS.AddResponse(response, data)
	glmmGSAPI.EndResponse()

	# Add offset
	if (!is.null(predictors$offset))
	{
		glmmGS.AddOffset(predictors$offset, data)
	}
	
	# Fixed effects
	nfixef <- length(predictors$fixef) 
	if (nfixef > 0L)
	{
		glmmGSAPI.BeginFixedEffects()
		for (i in 1L:nfixef)
		{
			# Set block
			block <- predictors$fixef[[i]]$block
			
			# Add block to API
			if (attr(block, "type") == "dense") 
			{
				# Dense block
				glmmGSAPI.BeginGlobalBlock()
				glmmGS.AddCovariates(block$covariates, data)
				glmmGSAPI.EndGlobalBlock()			
			}
			else if (attr(block, "type") == "stratified")
			{
				# Stratified block
				glmmGSAPI.BeginStratifiedBlock(block$factor$indices)
				glmmGS.AddCovariates(block$covariates, data)
				glmmGSAPI.EndStratifiedBlock()
			}
			else
			{
				stop("Unsupported block type")
			}
		}
		glmmGSAPI.EndFixedEffects()
	}
	
	# Random effects
	nranef <- length(predictors$ranef) 
	if (nranef > 0L)
	{
		glmmGSAPI.BeginRandomEffects()
		for (i in 1L:nranef)
		{
			# Set block
			block <- predictors$ranef[[i]]$block
			
			if (attr(block, "type") == "dense")
			{
				# Dense block
				glmmGSAPI.BeginGlobalBlock()
				glmmGS.AddCovariates(block$covariates, data)
				glmmGS.AddCovarianceModel(block$covariance.model, covariance.models)
				glmmGSAPI.EndGlobalBlock()			
			}
			else if (attr(block, "type") == "stratified")
			{
				# Stratified block
				glmmGSAPI.BeginStratifiedBlock(block$factor$indices)
				glmmGS.AddCovariates(block$covariates, data)
				glmmGS.AddCovarianceModel(block$covariance.model, covariance.models)
				glmmGSAPI.EndStratifiedBlock()
			}
			else
			{
				stop("Unsupported block type")
			}
		}
		glmmGSAPI.EndRandomEffects()		
	}
	
	# End model definition
	glmmGSAPI.EndModel()
	
	# Fit model
	start.time <- proc.time();
	glmmGSAPI.Fit(control)
	time <- proc.time() - start.time; 
	
	# Set results
	glmmGS <- list()
	class(glmmGS) <- "glmmGS"
	glmmGS$fixef <- glmmGSAPI.GetFixef(predictors$fixef)
	glmmGS$ranef <- glmmGSAPI.GetRanef(predictors$ranef)
	iterations <- glmmGSAPI.GetIterations()
	iterations <- list(outer = iterations[1L], coef = iterations[2L], vcomp = iterations[3L])
	glmmGS$iterations <- iterations;
	glmmGS$proc.time <- time[3L];
	
	# Tyde up
	glmmGSAPI.Tidy()
	if (nranef > 0L)
	{
		for (i in 1L:nranef)
			predictors$ranef[[i]]$block$factor$indices <- NULL
	}
	
	# Return
	glmmGS
}
