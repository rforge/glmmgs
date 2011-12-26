# Fit
glmmGS.Fit = function(control)
{
	# Fit model
	glmmGSAPI.Fit(control$reltol, control$abstol, control$maxit, control$verbose);
	
	# Retrieve results
	fixed.effects = glmmGSAPI.GetFixedEffectsCoefficients();
	random.effects = glmmGSAPI.GetRandomEffectsCoefficients();
	covariance.components = glmmGSAPI.GetCovarianceComponents();
	iterations = glmmGSAPI.GetIterations();
	return(list(fixed.effects = fixed.effects,
		random.effects = random.effects,
		covariance.components = covariance.components,
		iterations = iterations));
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
	else if (covariance_model$type == "SparsePrecisionModel")
	{
		glmmGSAPI.AddSparsePrecisionModel(covariance_model$R);
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
	env = environment(formula);
	
	# Convert formula and family into text
	formula = as.character(formula);
	family = family()$family;
	
	# Split response and predictor string
	response = formula[2];
	predictor = formula[3];

	# Clean-up state machine
	glmmGSAPI.Tidy();
	
	# Initialize state machine
	glmmGSAPI.Begin();
	
	# Add response
	glmmGS.AddResponse(response, family, data, env);
	
	# Initialize position counter
	pos = 1L;
	
	# Add offset
	offset = glmmGSParser.GetOffset(predictor, pos);
	if (is.null(offset) == FALSE)
	{
		pos = offset$pos;
		glmmGS.AddOffset(offset, data, env);
		pos = glmmGSParser.ParseSeparator(predictor, pos);
	}
	
	# Add predictor blocks
	while (pos != -1L)
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
