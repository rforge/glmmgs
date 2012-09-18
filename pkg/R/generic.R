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
			else if (class(var$value) %in% c("integer", "numeric"))
			{
				varnames[index] <- var$name
				index <- index + 1L
			}
			else if (class(var$value) == "matrix")
			{
				ncols <- ncol(var$value)
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
		levels <- levels(block$factor$value)
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
			else if (class(var$value) %in% c("integer", "numeric"))
			{
				upper <- index + nlevels - 1L
				varname <- paste(var$name, block$factor$name, sep = "|")
				varnames[index:upper] <- paste(varname, levels, sep = "=")
				index <- index + nlevels
			}
			else if (class(var$value) == "matrix")
			{
				ncols <- ncol(var$value)
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
		nlevels <- nlevels(block$factor$value)
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
			else if (class(var$value) %in% c("integer", "numeric"))
			{
				varnames[index] <- var$name
				index <- index + 1L
			}
			else if (class(var$value) == "matrix")
			{
				ncols <- ncol(var$value)
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

GetVCompVariance <- function(ranef)
{
	covariance.model <- ranef$block$covariance.model$value
	
	if (class(covariance.model) %in% c(
			"glmmGS.IdentityCovarianceModel",
			"glmmGS.PrecisionModel",
			"glmmGS.SparsePrecisionModel"))
	{
		vcomp <- ranef$vcomp
		var <- 1 / vcomp$estm
	}
	else
	{
		stop("Unsupported covariance model")
	}
	
	var
}

GetVCompVarianceInterval <- function(ranef)
{
	interval <- list();
	covariance.model <- ranef$block$covariance.model$value
	if (class(covariance.model) %in% c(
			"glmmGS.IdentityCovarianceModel",
			"glmmGS.PrecisionModel",
			"glmmGS.SparsePrecisionModel"))
	{
		vcomp <- ranef$vcomp
		lower <- 1 / (vcomp$estm + 2 * sqrt(diag(vcomp$vcov)))
		upper <- 1 / (vcomp$estm - 2 * sqrt(diag(vcomp$vcov)))
		upper <- ifelse(upper < 0, Inf, upper)
	}
	else
	{
		stop("Unsupported covariance model")
	}
	interval$lower <- lower;
	interval$upper <- upper;
	interval
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
			var <- GetVCompVariance(ranef)
			lu <- GetVCompVarianceInterval(ranef)
			
			# Generate summary table
			summary <- data.frame(var, lu$lower, lu$upper)
			dimnames(summary) <- list(names, c("Variance", "2.5%", "97.5%"))
			
			# Print summary table
			cat("\nBlock: \'", ranef$block$name, "\'\n", sep = "")
			cat("Variance Components:\n")
			print(summary, digits = digits, right = TRUE)
		}
	}
	
	cat("\nNumber of Gauss-Seidel iterations: ", glmmGS$iterations, "\n")
}
