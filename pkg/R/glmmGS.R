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
glmmGS.AddVariables = function(block, data, env)
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
			glmmGS.AddVariables(block$text, data, env);
			glmmGSAPI.EndGlobalBlock();			
		}
		else if (attr(block, "type") == "stratified")
		{
			# Stratified block
			glmmGSAPI.BeginStratifiedBlock(glmmGS.GetFactor(block$text, data, env));
			glmmGS.AddVariables(block$text, data, env);
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
			glmmGS.AddVariables(block$text, data, env);
			glmmGS.AddCovarianceModel(block$text, covariance.models, env);
			glmmGSAPI.EndGlobalBlock();			
		}
		else if (attr(block, "type") == "stratified")
		{
			# Stratified block
			glmmGSAPI.BeginStratifiedBlock(glmmGS.GetFactor(block$text, data, env));
			glmmGS.AddVariables(block$text, data, env);
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

# Get binomial response data
glmmGS.GetBinomialResponse = function(response, data, env)
{
	# Define variables
	y = NULL;
	counts = NULL;
	
	# Get variable names
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
	
	return(list(y = y, counts = counts));
}

# Get response (other than binomial) data
glmmGS.GetResponse = function(response, data, env)
{
	# Define variables
	y = NULL;
	
	# Get variable name
	y = glmmGS.GetVariable(glmmGSParser.Trim(response), data, env);
	
	# TODO: check that no other symbols are defined after variable name

	return(list(y = y));
}

# Add response to model
glmmGS.AddResponse = function(response, family, data, env)
{
	# Response
	glmmGSAPI.BeginResponse(family);
	if (family == "binomial")
	{
		bino = glmmGS.GetBinomialResponse(response, data, env);
		glmmGSAPI.AddResponse(bino$y);
		glmmGSAPI.AddCounts(bino$counts);
	}
	else if (family == "poisson")
	{
		pois = glmmGS.GetResponse(response, data, env);
		glmmGSAPI.AddResponse(pois$y);
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
	# Convert formula and family into text
	formula = as.character(formula);
	family = family()$family;
	
	# Split response and predictor string
	response = formula[2];
	predictor = formula[3];
	env = parent.frame();

	# Initialize state-machine
	glmmGSAPI.ForceEnd();
	glmmGSAPI.Begin();
	
	# Add response
	glmmGS.AddResponse(response, family, data, env);
	
	# Add predictor blocks
	pos = 1;
	repeat
	{
		block = glmmGSParser.GetNextBlock(predictor, pos);
		pos = block$pos;
		glmmGS.AddBlock(block, data, covariance.models, env);
		pos = glmmGSParser.SkipWhites(predictor, pos);
		token = glmmGSParser.GetToken(predictor, "\\(", pos);
		pos = token$pos;
		text = glmmGSParser.Trim(token$text);
		if (nchar(text) == 0)
			break;
		if (text != "+")
			stop("Wrong format");
	}
	
	# Fit model
	results = glmmGS.Fit(control);

	# Terminate state-machine
	glmmGSAPI.End();
	
	# Return results
	return(results);
}
