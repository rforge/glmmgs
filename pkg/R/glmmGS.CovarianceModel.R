# Construct covariance models
glmmGS.CovarianceModel <- function(type, ...)
{
	model <- NULL
	ls <- list(...)
	
	model <- list()
	
	if (type == "identity")
	{
		class(model) <- "glmmGS.IdentityCovarianceModel"
	}
	else if (type == "precision")
	{
		R <- ls[[1]]
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
	
	model
}
