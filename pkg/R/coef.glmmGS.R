coef.glmmGS <- function(object, ...)
{
	glmmGS <- object
	list <- list(...)
	if (length(list) == 0L || (length(list) == 1L && list[[1L]] == "fixef"))
	{
		n <- length(glmmGS$fixef)
		coef <- list()
		names <- character(n)
		for (i in 1L:n)
		{
			block <- glmmGS$fixef[[i]]
			coef[[i]] <- block$coef
			names[i] <- block$block$name
		}
		names(coef) <- names
	}
	else if (length(list) == 1L && list[[1L]] == "ranef")
	{
		n <- length(glmmGS$ranef)
		coef <- list()
		names <- character(n)
		for (i in 1L:n)
		{
			block <- glmmGS$ranef[[i]]
			coef[[i]] <- block$coef
			names[i] <- block$block$name
		}
		names(coef) <- names
	}
	else
	{
		stop("Invalid argument")
	}
	
	coef
}
