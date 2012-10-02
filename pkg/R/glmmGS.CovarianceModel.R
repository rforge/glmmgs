# Construct covariance models
glmmGS.CovarianceModel <- function(type, ...)
{
	# Get variable list
	ls <- list(...)
	
	# Get within parameter
	within <- ls$within
	if(is.null(within))
		within <- "diagonal"
	
	# Instantiate covariance model
	model <- list()
	
	# Build covariance model class
	if (type == "identity")
	{
		if (within == "diagonal")
		{
			class(model) <- "glmmGS.IdentityModel"
		}
		else if (within == "multivariate")
		{
			class(model) <- "glmmGS.MultivariateIdentityModel"
		}
		else
		{
			stop("Invalid \'within\' argument")
		}
	}
	else if (type == "precision")
	{
		R <- ls[[1L]]
		if (class(R) == "matrix" && nrow(R) == ncol(R))
		{
			class(model) <- "glmmGS.PrecisionModel" 
			model$R <- R;
		}
		else if (class(R) == "glmmGS.SparseMatrix")
		{
			class(model) <- "glmmGS.SparsePrecisionModel" 
			model$R <- R;
		}
		else
		{
			stop("Invalid precision matrix")
		}
	}
	else
	{
		stop("Unsupported covariance model")
	}
	
	# Set covariance components
	model$theta <- ls$fix.components
	if (is.null(model$theta))
		model$theta <- double(0L)

	model
}
