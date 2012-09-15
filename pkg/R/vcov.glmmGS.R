vcov.glmmGS <- function(object, ...)
{
	glmmGS <- object
	list <- list(...)
	if (length(list) == 0L || (length(list) == 1L && list[[1L]] == "fixef"))
	{
		n <- length(glmmGS$fixef)
		vcov <- list()
		names <- character(n)
		for (i in 1L:n)
		{
			block <- glmmGS$fixef[[i]]
			vcov[[i]] <- block$vcov
			names[i] <- block$block$name
		}
		names(vcov) <- names
	}
	else if (length(list) == 1L && list[[1L]] == "ranef")
	{
		n <- length(glmmGS$ranef)
		vcov <- list()
		names <- character(n)
		for (i in 1L:n)
		{
			block <- glmmGS$ranef[[i]]
			vcov[[i]] <- block$vcov
			names[i] <- block$block$name
		}
		names(vcov) <- names
	}
	else
	{
		stop("Invalid argument")
	}
	
	vcov
}
