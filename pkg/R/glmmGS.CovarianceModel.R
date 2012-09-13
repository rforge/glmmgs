# Construct covariance models
glmmGS.CovarianceModel <- function(type, ...)
{
	model <- NULL
	ls <- list(...)
	
	if (type == "identity")
	{
		model <- list()
		class(model) <- "IdentityCovarianceModel"
	}
	else if (type == "precision")
	{
		R <- ls[[1]]
		if (is.null(R))
		{
			stop("Invalid precision matrix")
		}
		else if (class(R) == "matrix" && nrow(R) == ncol(R))
		{
			model <- list(R = R);
			class(model) <- "PrecisionModel" 
		}
		else if (class(R) == "glmmGS.SparseMatrix")
		{
			model <- list(R = R);
			class(model) <- "SparsePrecisionModel" 
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
