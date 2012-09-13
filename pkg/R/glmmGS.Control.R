# Construct list of controls
glmmGS.Control <- function(reltol = 1.e-6, abstol = 1.e-6, maxit = 200, verbose = FALSE)
{
	control <- list(reltol = reltol, abstol = abstol, maxit = maxit, verbose = verbose)
	class(control) <- "glmmGS.Control"
	control
}
