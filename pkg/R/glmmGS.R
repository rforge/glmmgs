# Create factor variables
glmmGS.CreateFactors <- function(blocks, data)
{
	if (length(blocks) > 0L)
	{
		for (i in 1:length(blocks))
		{
			block <- blocks[[i]]
			if (attr(block, "type") == "stratified")
			{
				# Check that variable ifactor does not exists
				if (!is.null(block$ifactor)) stop("ifactor already defined")
				blocks[[i]]$ifactor <- as.integer(as.factor(get(block$factor, data))) - 1L
			}
		}
	}
	blocks
}

# Add variables to block
glmmGS.AddPredictors <- function(vars, data)
{
	for (varname in vars) 
	{
		if (varname == "1") 
		{
			glmmGSAPI.AddIntercept()
		}
		else 
		{
			glmmGSAPI.AddCovariate(get(varname, data))
		}
	}
}

# Add covariance model to model
glmmGS.AddCovarianceModel = function(cov.model, covariance.models)
{
	cov.model <- get(cov.model, covariance.models)
	
	if (cov.model$type == "IdentityCovarianceModel") 
	{
		glmmGSAPI.AddIdentityCovarianceModel()
	}
	else if (cov.model$type == "PrecisionModel")
	{
		glmmGSAPI.AddPrecisionModel(cov.model$R)
	}
	else if (cov.model$type == "SparsePrecisionModel") 
	{
		glmmGSAPI.AddSparsePrecisionModel(cov.model$R)
	}
	else
	{
		stop("Unsupported covariance model")
	}
}

# Fit
glmmGS.Fit = function(control)
{
	# Fit model
	glmmGSAPI.Fit(control$reltol, control$abstol, control$maxit, control$verbose)
	
	# Retrieve results
	fixed.effects = glmmGSAPI.GetFixedEffectsCoefficients()
	random.effects = glmmGSAPI.GetRandomEffectsCoefficients()
	covariance.components = glmmGSAPI.GetCovarianceComponents()
	iterations = glmmGSAPI.GetIterations()
	return(list(fixed.effects = fixed.effects,
					random.effects = random.effects,
					covariance.components = covariance.components,
					iterations = iterations))
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
	if (family$family == "binomial") 
	{
		if ((family$link %in% c("logit")) == FALSE) 
		{
			stop("unsupported link in binomial family")
		}
	}
	else if (family$family == "poisson") 
	{
		if ((family$link %in% c("log")) == FALSE) 
		{
			stop("unsupported link in poisson family")
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
	response <- glmmGSParser.GetResponse(formula, family$family)
	predictors <- glmmGSParser.GetPredictors(formula)
	
	# Add ifactor variable to stratified blocks
	predictors$fixef <- glmmGS.CreateFactors(predictors$fixef, data)
	predictors$ranef <- glmmGS.CreateFactors(predictors$ranef, data)
	
	# Initialize API
	glmmGSAPI.BeginModel()
	
	# Add response
	glmmGSAPI.BeginResponse(family$family)
	glmmGSAPI.AddResponse(get(response$vars[1], data))
	if (family$family == "binomial") 
	{
		if (length(response$vars) == 1L)
		{
			response.sizes <- integer(length(get(response$vars[1], data)))
		}
		else
		{
			response.sizes <- get(response$vars[2], data)
		}
		glmmGSAPI.AddCounts(response.sizes)
	}
	glmmGSAPI.EndResponse()

	# Add offset
	if (!is.null(predictors$offset))
		glmmGSAPI.AddOffset(get(predictors$offset, data))
	
	# Fixed effects
	if (length(predictors$fixef) > 0L)
	{
		glmmGSAPI.BeginFixedEffects()
		for (block in predictors$fixef)
		{
			if (attr(block, "type") == "dense") 
			{
				# Dense block
				glmmGSAPI.BeginGlobalBlock()
				glmmGS.AddPredictors(block$vars, data)
				glmmGSAPI.EndGlobalBlock()			
			}
			else if (attr(block, "type") == "stratified")
			{
				# Stratified block
				glmmGSAPI.BeginStratifiedBlock(block$ifactor)
				glmmGS.AddPredictors(block$vars, data)
				glmmGSAPI.EndStratifiedBlock()			
			}
		}
		glmmGSAPI.EndFixedEffects()
	}
	
	# Random effects
	if (length(predictors$ranef) > 0L)
	{
		glmmGSAPI.BeginRandomEffects()
		for (block in predictors$ranef)
		{
			if (attr(block, "type") == "dense")
			{
				# Dense block
				glmmGSAPI.BeginGlobalBlock()
				glmmGS.AddPredictors(block$vars, data)
				glmmGS.AddCovarianceModel(block$cov.model, covariance.models)
				glmmGSAPI.EndGlobalBlock()			
			}
			else if (attr(block, "type") == "stratified")
			{
				# Stratified block
				glmmGSAPI.BeginStratifiedBlock(block$ifactor)
				glmmGS.AddPredictors(block$vars, data)
				glmmGS.AddCovarianceModel(block$cov.model, covariance.models)
				glmmGSAPI.EndStratifiedBlock()			
			}
		}
		glmmGSAPI.EndRandomEffects()		
	}
	
	# End model definition
	glmmGSAPI.EndModel()
	
	# Fit model
	results <- glmmGS.Fit(control)
	
	# Return results
	return(results)
}
