# Construct control
glmmGS.Control = function(reltol = 1.e-6, abstol = 1.e-6, maxit = 50)
{
	x = list(reltol = reltol, abstol = abstol, maxit = maxit);
	return(x);
}

# Construct identity covariance model
glmmGS.IdentityCovarianceModel = function()
{
	x = list(type = "IdentityCovarianceModel");
	return(x);
}

# Construct precision model
glmmGS.PrecisionModel = function(R)
{
	x = list(type = "PrecisionModel", R = R);
	return(x);
}

# Construct covariance models
glmmGS.CovarianceModel = function(type, ...)
{
	V = NULL;
	ls = list(...);
	cat(names(ls));
	if (type == "identity")
	{
		V = glmmGS.IdentityCovarianceModel();
	}
	else if (type == "precision")
	{
		V = glmmGS.PrecisionModel(ls[["R"]]);
	}
	else
	{
		stop("Unsupported covariance model");
	}
	return(V);
}

# Fit
glmmGS.Fit = function(control)
{
	# Fit model
	glmmGSAPI.Fit(control$reltol, control$abstol, control$maxit);
	
	# Retrieve results
	beta = glmmGSAPI.GetFixedEffectsCoefficients();
	theta = glmmGSAPI.GetVarianceComponents();
	return(list(beta = beta, theta = theta));
}

# Get variable
glmmGS.GetVariable = function(name, data, env)
{
	x = NULL;
	if (!is.null(data))
		x = data[[name]];
	if (is.null(x))
		x = get(name, envir = env);
	if (is.null(x))
		stop(paste("Variable '", name, "' not found", sep = ""));
	return(x);
}

# Add covariance model to model
glmmGS.AddCovarianceModel = function(block, covariance.models, env)
{
	pos = 1;
	pos = glmmGSParser.Find(block, "~", pos) + 1;
	token = glmmGSParser.GetToken(block, "", pos);
	varname = glmmGSParser.Trim(token$text);
	covariance_model = glmmGS.GetVariable(varname, covariance.models, env);
	
	if (covariance_model$type == "IdentityCovarianceModel")
	{
		glmmGSAPI.AddIdentityCovarianceModel();
	}
	else if (covariance_model$type == "PrecisionModel")
	{
		glmmGSAPI.AddPrecisionModel(covariance_model$R);
	}
	else
	{
		stop("Invalid covariance model");
	}
}

# Add variables to block
glmmGS.AddPredictors = function(block, data, env)
{
	token = glmmGSParser.GetToken(block, "(\\|)|(~)", 1);
	block = token$text;
	pos = 1;
	repeat
	{
		pos = glmmGSParser.SkipWhites(block, pos);
		token = glmmGSParser.GetToken(block, "\\+", pos);
		varname = glmmGSParser.Trim(token$text);
		if (varname == "1")
		{
			glmmGSAPI.AddIntercept();
		}
		else
		{
			glmmGSAPI.AddCovariate(glmmGS.GetVariable(varname, data, env));
		}
		if (token$pos == -1)
			break;
		pos = token$pos + 1;
	}
}

# Get factor data
glmmGS.GetFactor = function(block, data, env)
{
	pos = 1;
	pos = glmmGSParser.Find(block, "\\|", pos) + 1;
	token = glmmGSParser.GetToken(block, "~", pos);
	varname = glmmGSParser.Trim(token$text);
	return(glmmGS.GetVariable(varname, data, env));
}

# Add offset to model
glmmGS.AddOffset = function(offset, data, env)
{
	# Get response
	varname = glmmGSParser.Trim(offset$text);
	offset = glmmGS.GetVariable(varname, data, env);
	
	# Add offset
	glmmGSAPI.AddOffset(offset)
}
	
# Add predictor block to model
glmmGS.AddBlock = function(block, data, covariance.models, env)
{
	if (attr(block, "effects") == "fixed")
	{
		# Fixed effects
		glmmGSAPI.BeginFixedEffects();
		if (attr(block, "type") == "global")
		{
			# Global block
			glmmGSAPI.BeginGlobalBlock();
			glmmGS.AddPredictors(block$text, data, env);
			glmmGSAPI.EndGlobalBlock();			
		}
		else if (attr(block, "type") == "stratified")
		{
			# Stratified block
			glmmGSAPI.BeginStratifiedBlock(glmmGS.GetFactor(block$text, data, env));
			glmmGS.AddPredictors(block$text, data, env);
			glmmGSAPI.EndStratifiedBlock();			
		}
		else
		{
			stop("Invalid fixed-effects block type");
		}
		glmmGSAPI.EndFixedEffects();
	}
	else if (attr(block, "effects") == "random")
	{
		# Random effects
		glmmGSAPI.BeginRandomEffects();
		if (attr(block, "type") == "global")
		{
			# Global block
			glmmGSAPI.BeginGlobalBlock();
			glmmGS.AddPredictors(block$text, data, env);
			glmmGS.AddCovarianceModel(block$text, covariance.models, env);
			glmmGSAPI.EndGlobalBlock();			
		}
		else if (attr(block, "type") == "stratified")
		{
			# Stratified block
			glmmGSAPI.BeginStratifiedBlock(glmmGS.GetFactor(block$text, data, env));
			glmmGS.AddPredictors(block$text, data, env);
			glmmGS.AddCovarianceModel(block$text, covariance.models, env);
			glmmGSAPI.EndStratifiedBlock();			
		}
		else
		{
			stop("Invalid random-effects block type");
		}
		glmmGSAPI.EndRandomEffects();		
	}
	else
	{
		stop("Invalid 'effects' attribute");
	}
}

# Add response to model
glmmGS.AddResponse = function(response, family, data, env)
{
	# Response
	glmmGSAPI.BeginResponse(family);
	if (family == "binomial")
	{
		# Get response and counts
		pos = 1;
		pos = glmmGSParser.SkipWhites(response, pos);
		pos = glmmGSParser.Match(response, "\\(", pos);
		
		pos = glmmGSParser.SkipWhites(response, pos);
		token = glmmGSParser.GetToken(response, "\\|", pos);
		pos = token$pos + 1;
		y = glmmGS.GetVariable(glmmGSParser.Trim(token$text), data, env);
		
		pos = glmmGSParser.SkipWhites(response, pos);
		token = glmmGSParser.GetToken(response, "\\)", pos);
		pos = token$pos + 1;
		counts = glmmGS.GetVariable(glmmGSParser.Trim(token$text), data, env);
		
		pos = glmmGSParser.SkipWhites(response, pos);
		token = glmmGSParser.GetToken(response, ".", pos);
		if (token$pos != -1)
			stop("Invalid response formula");
		
		# Add response and counts
		glmmGSAPI.AddResponse(y);
		glmmGSAPI.AddCounts(counts);
	}
	else if (family == "poisson")
	{
		# Get response
		varname = glmmGSParser.Trim(response);
		y = glmmGS.GetVariable(varname, data, env);
		
		# Add response
		glmmGSAPI.AddResponse(y);
	}
	else
	{
		stop("Unsupported family");
	}
	glmmGSAPI.EndResponse();
}

# Main function
glmmGS = function(formula, family, data = NULL, covariance.models = NULL, control = glmmGS.Control())
{
	# Gets environment of calling function
	env = parent.frame();
	
	# Convert formula and family into text
	formula = as.character(formula);
	family = family()$family;
	
	# Split response and predictor string
	response = formula[2];
	predictor = formula[3];

	# Initialize state-machine
	glmmGSAPI.ForceEnd();
	glmmGSAPI.Begin();
	
	# Add response
	glmmGS.AddResponse(response, family, data, env);
	
	# Initialize position counter
	pos = 1;
	
	# Add offset
	offset = glmmGSParser.GetOffset(predictor, pos);
	if (is.null(offset) == FALSE)
	{
		pos = offset$pos;
		glmmGS.AddOffset(offset, data, env);
		pos = glmmGSParser.ParseSeparator(predictor, pos);
	}
	
	# Add predictor blocks
	while (pos != -1)
	{
		# Add block
		block = glmmGSParser.GetNextBlock(predictor, pos);
		pos = block$pos;
		glmmGS.AddBlock(block, data, covariance.models, env);

		# Parse separator and set position to next block 
		pos = glmmGSParser.ParseSeparator(predictor, pos);
	}
	
	# Fit model
	results = glmmGS.Fit(control);
	
	# Terminate state-machine
	glmmGSAPI.End();
	
	# Return results
	return(results);
}
