# fixef
fixef <- function(object, ...)
{
	UseMethod("fixef")
}

fixef.glmmGS <- function(object, ...)
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

# coef
coef.glmmGS <- function(object, ...)
{
	fixef.glmmGS(object, list(...))
}

# vcov
vcov.glmmGS <- function(object, ...)
{
	glmmGS <- object
	
	n <- length(glmmGS$fixef)
	if (n > 0L)
	{
		vcov <- list()
		names <- character(n)
		for (i in 1L:n)
		{
			block <- glmmGS$fixef[[i]]
			vcov[[i]] <- block$coef$vcov
			names[i] <- block$block$name
		}
		names(vcov) <- names
	}	
	else
	{
		vcov <- NA
	}
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
	
	n <- length(glmmGS$ranef)
	if (n > 0L)
	{
		vcomp <- list()
		names <- character(n)
		for (i in 1L:n)
		{
			block <- glmmGS$ranef[[i]]
			vcomp[[i]] <- block$vcomp$estm
			names[i] <- block$block$name
		}
		names(vcomp) <- names
	}	
	else
	{
		vcomp <- NA
	}
	vcomp
}

# Utilities functions for print.glmmGS
GetFixefNames <- function(fixef)
{
	block <- fixef$block
	if (attr(block, "type") == "dense")
	{
		nvars <- GetNumberOfVariables(block)
		varnames <- character(nvars)
		index <- 1L
		for (var in block$covariates)
		{
			if (var$name == "1")
			{
				varnames[index] <- "(Intercept)"
				index <- index + 1L
			}
			else if (var$class %in% c("integer", "numeric"))
			{
				varnames[index] <- var$name
				index <- index + 1L
			}
			else if (var$class == "matrix")
			{
				ncols <- var$dim
				if (ncols > 0L)
				{
					for (j in 1L:ncols)
					{
						varnames[index] <- sprintf("%s[,%d]", var$name, j)
						index <- index + 1L
					}
				}
			}
			else
			{
				stop("Unsupported covariate type")
			}
		}
	}
	else if (attr(block, "type") == "stratified")
	{
		nvars <- GetNumberOfVariables(block)
		levels <- block$factor$levels
		nlevels <- length(levels)
		varnames <- character(nvars * nlevels)
		index <- 1L
		for (var in block$covariates)
		{
			if (var$name == "1")
			{
				upper <- index + nlevels - 1L
				#varname <- paste("(Intercept)", block$factor$name, sep = "|")
				varname <- block$factor$name
				varnames[index:upper] <- paste(varname, levels, sep = "=")
				index <- index + nlevels
			}
			else if (var$class %in% c("integer", "numeric"))
			{
				upper <- index + nlevels - 1L
				varname <- paste(var$name, block$factor$name, sep = "|")
				varnames[index:upper] <- paste(varname, levels, sep = "=")
				index <- index + nlevels
			}
			else if (var$class == "matrix")
			{
				ncols <- var$dim
				if (ncols > 0L)
				{
					for (j in 1L:ncols)
					{
						varname <- sprintf("%s[,%d]|%s", var$name, j, block$factor$name)
						upper <- index + nlevels - 1L
						varnames[index:upper] <- paste(varname, levels, sep = "=")
						index <- index + nlevels
					}
				}
			}
			else
			{
				stop("Unsupported covariate type")
			}
		}
	}
	else
	{
		stop("Unsupported type")
	}
	varnames
}

GetFixefSE <- function(fixef)
{
	block <- fixef$block
	coef <- fixef$coef
	if (attr(block, "type") == "dense")
	{
		se <- sqrt(diag(coef$vcov))
	}
	else if (attr(block, "type") == "stratified")
	{
		nvars <- GetNumberOfVariables(block)
		nlevels <- length(block$factor$levels)
		se <- double(nvars * nlevels)
		for (j in 1L:nvars)
		{
			lower <- nlevels * (j - 1L) + 1L
			upper <- lower + nlevels - 1L
			se[lower:upper] <- sqrt(coef$vcov[, j, j])
		}
	}
	else
	{
		stop("Unsupported type")
	}
	
	se
}

GetVCompNames <- function(ranef)
{
	block <- ranef$block
	if (attr(block, "type") == "dense")
	{
		varnames <- unlist(strsplit(block$name, "~"))[1]
	}
	else if (attr(block, "type") == "stratified")
	{
		nvars <- GetNumberOfVariables(block)
		varnames <- character(nvars)
		index <- 1L
		for (var in block$covariates)
		{
			if (var$name == "1")
			{
				varnames[index] <- "(Intercept)"
				index <- index + 1L
			}
			else if (var$class %in% c("integer", "numeric"))
			{
				varnames[index] <- var$name
				index <- index + 1L
			}
			else if (var$class == "matrix")
			{
				ncols <- var$dim
				if (ncols > 0L)
				{
					for (j in 1L:ncols)
					{
						varnames[index] <- sprintf("%s[,%d]", var$name, j)
						index <- index + 1L
					}
				}
			}
			else
			{
				stop("Unsupported covariate type")
			}
		}
	}
	else
	{
		stop("Unsupported type")
	}
	varnames
}

GetVComp <- function(ranef)
{
	vcomp <- list(var = list());
	covariance.model <- ranef$block$covariance.model
	if (covariance.model$class %in% c(
			"glmmGS.MultivariateIdentityModel"))
	{
		nvar <- GetNumberOfVariables(ranef$block)
		
		# Retrieve estimated precision matrix
		prec <- matrix(0, nvar, nvar)
		prec[lower.tri(prec, diag = TRUE)] <- ranef$vcomp$estm
		d <- as.vector(diag(prec))
		nd <- length(d)
		prec <- prec + t(prec) - diag(d, nd, nd)
		
		# Evaluate estimated variance matrix
		cov <- solve(prec)
		
		# TODO: evaluate se of covariance 
		vcomp$var$estm <- diag(cov)
		vcomp$var$upper <- NA
		vcomp$var$lower <- NA
		vcomp$cor <- cov2cor(cov)
		
	}
	else if (covariance.model$class %in% c(
			"glmmGS.IdentityModel",
			"glmmGS.PrecisionModel",
			"glmmGS.SparsePrecisionModel"))
	{
		prec <- ranef$vcomp$estm
		se <- sqrt(diag(ranef$vcomp$vcov))
		estm <- 1 / prec
		lower <- 1 / (prec + 2 * se)
		upper <- 1 / (prec - 2 * se)
		upper <- ifelse(upper < 0, Inf, upper)
		
		vcomp$var$estm <- estm;
		vcomp$var$lower <- lower;
		vcomp$var$upper <- upper;
	}
	else
	{
		stop("Unsupported covariance model")
	}
	
	vcomp
}

# print
print.glmmGS <- function(x, ...)
{
	glmmGS <- x
	digits <- 4L
	
	if (length(glmmGS$fixef) > 0L)
	{
		# Print fixed effects
		for (fixef in glmmGS$fixef)
		{
			names <- GetFixefNames(fixef)
			se <- GetFixefSE(fixef)
			estm <- as.vector(fixef$coef$estm)
			
			zval <- estm / se
			pval <- 2 * (1 - pnorm(abs(zval)))
			
			# P value string
			eps <- 2.e-16
			pstr <- ifelse(pval < eps, sprintf(" < %.3g", eps), sprintf("%.3g", pval))
			
			# Significance codes
			signif.values <- c(0, 0.001, 0.01, 0.05, 0.1, 1)
			signif.codes <- c("***", "** ", "*  ", ".  ", "") 
			intervals <- findInterval(pval, signif.values, all.inside = TRUE)
			sstr <- signif.codes[intervals]
			
			# Generate summary table
			summary <- data.frame(estm, se, zval, pstr, sstr)
			dimnames(summary) <- list(names, c("Estimate", "Std. Error", "z value", "Pr(>|z|)", ""))
			
			# Print summary table
			cat("\nBlock: \'", fixef$block$name, "\'\n", sep = "")
			cat("Fixed Effects:\n")
			print(summary, digits = digits)
		}
		cat("\nSignif. codes:  0 \'***\' 0.001 \'**\' 0.01 \'*\' 0.05 \'.\' 0.1 \' \' 1\n")
	}

	if (length(glmmGS$ranef) > 0L)
	{
		# Print random effects
		for (ranef in glmmGS$ranef)
		{
			names <- GetVCompNames(ranef)
			vcomp <- GetVComp(ranef)
			
			# Generate variance table
			summary <- data.frame(vcomp$var$estm, vcomp$var$lower, vcomp$var$upper)
			dimnames(summary) <- list(names, c("Variance", "2.5%", "97.5%"))
			
			# Print variance table
			cat("\nBlock: \'", ranef$block$name, "\'\n", sep = "")
			cat("Variance:\n")
			print(summary, digits = digits, right = TRUE)

			if (!is.null(vcomp$cor))
			{
				# Generate correlation table
				summary <- data.frame(vcomp$cor)
				dimnames(summary) <- list(names, names)
				
				# Print correlation table
				cat("Correlation:\n")
				print(summary, digits = digits, right = TRUE)
			}
		}
	}
	
	cat("\nNumber of coefficient updates: ", glmmGS$iterations$coef)
	cat("\nNumber of variance component updates: ", glmmGS$iterations$vcomp)
	cat("\nNumber of coefficient-variance iterations: ", glmmGS$iterations$outer)
	cat("\nProcess time: ", glmmGS$proc.time, "\n\n")
}
