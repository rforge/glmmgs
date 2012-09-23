# Add variables to block
glmmGS.AddCovariates <- function(covariates)
{
	for (var in covariates) 
	{
		if (var$name == "1") 
		{
			glmmGSAPI.AddIntercept(var$duplicate)
		}
		else 
		{
			glmmGSAPI.AddCovariate(var$value, var$duplicate)
		}
	}
}

# Add covariance model to model
glmmGS.AddCovarianceModel <- function(covariance.model)
{
	if (class(covariance.model) == "glmmGS.IdentityCovarianceModel") 
	{
		glmmGSAPI.AddIdentityCovarianceModel(covariance.model$S)
	}
	else if (class(covariance.model) == "glmmGS.PrecisionModel")
	{
		glmmGSAPI.AddPrecisionModel(covariance.model$R, covariance.model$S)
	}
	else if (class(covariance.model) == "glmmGS.SparsePrecisionModel") 
	{
		glmmGSAPI.AddSparsePrecisionModel(covariance.model$R, covariance.model$S)
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
	response <- glmmGS.Response(formula, family, data)
	predictors <- glmmGS.Predictors(formula, data, covariance.models)
	
	# Initialize API
	glmmGSAPI.BeginModel()
	
	# Add response
	glmmGSAPI.BeginResponse(family$family)
	glmmGSAPI.AddResponse(response$value)
	if (family$family == "binomial")
	{
		glmmGSAPI.AddCounts(response$size)
	}
	glmmGSAPI.EndResponse()

	# Add offset
	if (!is.null(predictors$offset))
	{
		glmmGSAPI.AddOffset(predictors$offset$value)
	}
	
	# Fixed effects
	if (length(predictors$fixef) > 0L)
	{
		glmmGSAPI.BeginFixedEffects()
		for (i in 1:length(predictors$fixef))
		{
			# Set block
			block <- predictors$fixef[[i]]$block
			
			# Add block to API
			if (attr(block, "type") == "dense") 
			{
				# Dense block
				glmmGSAPI.BeginGlobalBlock()
				glmmGS.AddCovariates(block$covariates)
				glmmGSAPI.EndGlobalBlock()			
			}
			else if (attr(block, "type") == "stratified")
			{
				# Stratified block
				glmmGSAPI.BeginStratifiedBlock(block$factor$indices)
				glmmGS.AddCovariates(block$covariates)
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
	if (length(predictors$ranef) > 0L)
	{
		glmmGSAPI.BeginRandomEffects()
		for (i in 1:length(predictors$ranef))
		{
			# Set block
			block <- predictors$ranef[[i]]$block
			
			if (attr(block, "type") == "dense")
			{
				# Dense block
				glmmGSAPI.BeginGlobalBlock()
				glmmGS.AddCovariates(block$covariates)
				glmmGS.AddCovarianceModel(block$covariance.model$value)
				glmmGSAPI.EndGlobalBlock()			
			}
			else if (attr(block, "type") == "stratified")
			{
				# Stratified block
				glmmGSAPI.BeginStratifiedBlock(block$factor$indices)
				glmmGS.AddCovariates(block$covariates)
				glmmGS.AddCovarianceModel(block$covariance.model$value)
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
	# Avoid deep copies
	glmmGS <- list()
	class(glmmGS) <- "glmmGS"
	glmmGS$fixef <- glmmGSAPI.GetFixef(predictors$fixef)
	glmmGS$ranef <- glmmGSAPI.GetRanef(predictors$ranef)
	iterations <- glmmGSAPI.GetIterations()
	iterations <- list(coef = iterations[1], 
			vcomp = iterations[2], 
			outer = iterations[3])
	glmmGS$iterations <- iterations;
	glmmGS$proc.time <- time[3];
	glmmGS
}
