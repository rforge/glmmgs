# Construct covariance models
glmmGS.CovarianceModel = function(type, ...)
{
	V = NULL;
	ls = list(...);
	if (type == "identity")
	{
		V = list(type = "IdentityCovarianceModel");
	}
	else if (type == "precision")
	{
		V = NULL;
		R = ls[[1]];
		if (is.null(R) == TRUE)
		{
			stop("Invalid precision matrix");
		}
		else if ((is.matrix(R) == TRUE) && (nrow(R) == ncol(R)))
		{
			V = list(type = "PrecisionModel", R = R);
		}
		else if (is.null(attr(R, "sparse.matrix", TRUE)) == FALSE)
		{
			V = list(type = "SparsePrecisionModel", R = R);
		}
		else
		{
			stop("Invalid precision matrix");
		}
	}
	else
	{
		stop("Unsupported covariance model");
	}
	return(V);
}
