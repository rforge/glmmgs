# Construct covariance models
glmmGS.CovarianceModel <- function(type, ...)
{
	# Get variable list
	ls <- list(...)
	
	# Instantiate covariance model
	model <- list()
	
	# Build covariance model class
	if (type == "identity")
	{
		class(model) <- "glmmGS.IdentityCovarianceModel"
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
	ifelse(!is.null(ls$fix.components), model$S <- ls$fix.components, model$S <- matrix(0, ncol = 0L, nrow = 0L))

	model
}
