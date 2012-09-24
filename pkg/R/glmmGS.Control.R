# Helper functions
ValidateControl <-function(reltol, abstol, maxit, verbose, max.update, max.value)
{
	ValidateDouble(reltol, 1L)
	ValidateDouble(abstol, 1L)
	ValidateInteger(maxit, 2L)
	ValidateInteger(verbose, 1L)
	ValidateDouble(max.update, 3L)
	ValidateDouble(max.value, 3L)
}

# Construct list of controls
glmmGS.Control <- function(
		reltol = 1.e-6, 
		abstol = 1.e-6,
		maxit = c(200L, 1L),
		verbose = FALSE,
		max.update = c(0, 0, 0),
		max.value = c(0, 0, 0))
{
	# Retrieve controls
	reltol <- as.double(reltol) 
	abstol <- as.double(abstol)
	if (length(maxit) == 1L)
	{
		maxit <- as.integer(c(maxit, 1L))
	}
	else
	{
		maxit <- as.integer(maxit)
	}
	max.update <- as.double(max.update)
	max.value <- as.double(max.value)
	verbose <- as.integer(verbose)

	# Validate controls
	ValidateControl(reltol, abstol, maxit, verbose, max.update, max.value)
	
	# Build control list
	control <- list(
			reltol = reltol, 
			abstol = abstol,
			maxit = maxit, 
			verbose = verbose,
			max.update = max.update,
			max.value = max.value)
	class(control) <- "glmmGS.Control"
	control
}
