# coef
coef.glmmGS <- function(object, ...)
{
	glmmGS <- object

	coef <- list()
	n <- length(glmmGS$fixef)
	if (n > 0L)
	{
		names <- character(n)
		for (i in 1L:n)
		{
			block <- glmmGS$fixef[[i]]
			coef[[i]] <- block$coef$estm
			names[i] <- block$block$name
		}
		names(coef) <- names
	}
	coef
}

# fixef
fixef <- function(object, ...)
{
	UseMethod("fixef")
}

fixef.glmmGS <- function(object, ...)
{
	coef.glmmGS(object, list(...))
}

# ranef
ranef <- function(object, ...)
{
	UseMethod("ranef")
}

ranef.glmmGS <- function(object, ...)
{
	glmmGS <- object
	
	coef <- list()
	n <- length(glmmGS$ranef)
	if (n > 0L)
	{
		names <- character(n)
		for (i in 1L:n)
		{
			block <- glmmGS$ranef[[i]]
			coef[[i]] <- block$coef$estm
			names[i] <- block$block$name
		}
		names(coef) <- names
	}
	coef
}

# vcov
vcov.glmmGS <- function(object, ...)
{
	glmmGS <- object
	
	n <- length(glmmGS$fixef)
	vcov <- list()
	names <- character(n)
	for (i in 1L:n)
	{
		block <- glmmGS$fixef[[i]]
		vcov[[i]] <- block$coef$vcov
		names[i] <- block$block$name
	}
	names(vcov) <- names
	
	vcov
}

# vcomp
vcomp <- function(object, ...)
{
	UseMethod("vcomp")
}

vcomp.glmmGS <- function(object, ...)
{
	glmmGS <- object
	
	vcomp <- list()
	n <- length(glmmGS$ranef)
	if (n > 0L)
	{
		names <- character(n)
		for (i in 1L:n)
		{
			block <- glmmGS$ranef[[i]]
			vcomp[[i]] <- block$vcomp$estm
			names[i] <- block$block$name
		}
		names(vcomp) <- names
	}
	vcomp
}

# print
print.glmmGS <- function(object, ...)
{
	cat("To be implemented!")
}
