# Helper functions
ValidateControl <-function(reltol, abstol, max.update, max.value, maxit, verbose)
{
	ValidateDouble(reltol, 1L)
	ValidateDouble(abstol, 1L)
	ValidateDouble(max.update, 3L)
	ValidateDouble(max.value, 3L)
	ValidateInteger(maxit, 2L)
	ValidateInteger(verbose, 1L)
}

# Construct list of controls
glmmGS.Control <- function(reltol = 1.e-6, abstol = 1.e-6,
		maxit = list(200L, 200L),
		verbose = FALSE,
		max.update = c(0, 0, 0),
		max.value = c(0, 0, 0))
{
	# Retrieve controls
	reltol <- as.double(reltol) 
	abstol <- as.double(abstol)
	if (length(maxit) == 1L)
	{
		maxit <- as.integer(c(maxit, maxit))
	}
	verbose <- as.integer(verbose)

	# Validate controls
	ValidateControl(reltol, abstol, max.update, max.value, maxit, verbose)
	
	# Build control list
	control <- list(
			reltol = reltol, 
			abstol = abstol,
			max.update = max.update,
			max.value = max.value,
			maxit = maxit, 
			verbose = verbose)
	class(control) <- "glmmGS.Control"
	control
}
