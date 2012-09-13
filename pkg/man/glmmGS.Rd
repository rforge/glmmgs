\name{glmmGS}
\alias{glmmGS}
\title{
	Fit Generalized Linear Mixed Models
}
\description{
	glmmGS is used to fit generalized mixed linear models specified by a string-like formula.
	The MLE estimation of the parameters is performed by using a Gauss-Seidel algorithm 
	to fit the regression coefficients (see Reference), and a Penalized Quasi Likelihood (PQL) 
	approach to fit the covariance components (see Note). The algorithm is highly optimized to fit 
	fixed effect models to data-sets with a large number of observations and 
	with stratified covariates with a large number of levels, and mixed models 
	with stratified covariates with a large number of levels and diagonal
	covariance structures. Covariance structures defined by non diagonal dense 
	or sparse precision matrices are also allowed (see \code{\link{glmmGS.CovarianceModel}}).
}
\usage{
	glmmGS(formula, family, data, covariance.models, control = glmmGS.Control())
}
%- maybe also 'usage' for other objects documented here.
\arguments{
	\item{formula}{
		a formula object providing a symbolic description of the model to be fitted.
		The formula object must have the format \emph{response ~ predictor}. 
		Discrete responses must be defined by integer vectors. For a binomial
		family, the \emph{response} is specified by the pair \code{(response/size)}, with or without parentheses.
		Both \code{response} and \code{size} must be integer vectors. If \code{size} is omitted,
		a constant \code{size} of \code{1}'s is assumed.
		The \emph{predictor} specifies both fixed and random effect covariates. 
		The covariates must be grouped in blocks (the blocks of the Gauss-Seidel algorithm, see Reference) 
		specified by pairs of parentheses. The covariates in a block can be stratified by using the 
		vertical bar symbol \code{|} followed by the name of a grouping variable, 
		defined either by a factor or an object coercible to a factor.
		The covariance model inside the random effect blocks are specified by using the tilde symbol 
		\code{~} followed by the name of a \code{\link{glmmGS.CovarianceModel}} object.
		If an offset \code{off} is present in the linear predictor, it must be specified with keyword
		\code{offset(off)}. Both fixed and random intercepts must be explicitly specified 
		using the the symbol \code{1}; implicit specification
		of the intercept is not assumed. If \code{data} or \code{covariance.models}
		are specified, then variable lookup is performed only in
		\code{data} and \code{covariance.models}. If \code{data} or \code{covariance.model}
		are missing, then the variable lookup is performed in the environment of the 
		\code{formula} object.  
	}
	\item{family}{
		a description of the error distribution. Only two families are currently allowed:
		\code{binomial} and \code{poisson} with a canonical link.
	}
	\item{data}{
		an optional data frame, list or environment (or object coercible to a data frame) 
		containing the variables in the model. 
		If not found in \code{data}, the variables are taken from \code{environment(formula)}, 
		typically the environment from which \code{glmmGS} is called.
	}
	\item{covariance.models}{
		an optional list or environment containing the covariance models referenced 
		in the formula. If not found in \code{covariance.models}, the covariance models are taken from 
		\code{environment(formula)}, typically the environment from which \code{glmmGS} is called.
	}
	\item{control}{
		a list of parameters for controlling the fitting process. The list is returned
		by the \code{\link{glmmGS.Control}} function.
	}
}
\details{
	%%  ~~ If necessary, more details than the description above ~~
}
\value{
	%%  ~Describe the value returned
	%%  If it is a LIST, use
	%%  \item{comp1 }{Description of 'comp1'}
	%%  \item{comp2 }{Description of 'comp2'}
	%% ...
	an object of type \code{glmmGS} containing: (1) the estimated fixed effects;
	(2) the estimated random effects; (3) the diagonal blocks of the 
	fixed effect covariance matrix according to the GS blocks
	specified in the formula; (4) the covariance matrix of the random effects;
	(5) the estimated working weights, predictors, and other control parameters.
}

\references{
	Guha, S., Ryan, L., Morara M. (2009) Gauss-Seidel Estimation of Generalized Linear
	Mixed Models With Application to Poisson Modeling of Spatially Varying Disease Rates. 
	\emph{Journal of Computational and Graphical Statistics}, \bold{4}, 810--837.
}
\author{
	Michele Morara, Louise Ryan, Subharup Guha, Christopher Paciorek
}
\note{
%%  ~~further notes~~
	The distinguishing characteristic of the Gauss-Seidel algorithm is that the full
	precision matrix of the posterior distribution of the parameters
	is never computed. Only diagonal blocks of the precision matrix are computed, 
	one for each pair of parentheses in the formula. Furthermore, each precision-block
	associated with stratified covariates, is subdivided in as many sub-blocks 
	as many levels there are in the grouping factor. As a consequence, 
	the standard errors associated with the coefficient estimates are computed 
	for each block conditionally to the other blocks, and they tend to be
	smaller than the standard-errors calculated using the full posterior
	distribution of the parameters. 
	
	The \code{glmmGS} function is optimized to handle data-sets with a large number of observations 
	and data stratifications with a large number of levels. 
	To avoid unnecessary memory allocation, the return value contains only minimal fitting information.
	Auxiliary functions to calculate quantities such as	block covariance matrices, 
	fitted values, predictors, will be soon released. We strongly recommend not to commit 
	to the current format of the return value, since it will likely change in the next release. 
	A user should write \sQuote{accessor} functions to parse the return value, so that user-code 
	using \code{glmmGS} would only need to change the \sQuote{accessor} functions in the next
	release. 
	
	Although it is possible to specify as many blocks of random effects as desired, the PQL update
	of the covariance components is performed one block at a time. This yields to different
	estimates compared to a joint PQL update of the covariance components. Alternative
	approaches, more compatible with the component-wise nature of the Gauss-Seidel
	algorithm, to the PQL approximation of the integration over the random effect 
	coefficients, are under consideration.
}

%% ~Make other sections like Warning with \section{Warning }{....} ~

\seealso{
%% ~~objects to See Also as \code{\link{help}}, ~~~
\code{\link{glmmGS.CovarianceModel}}, \code{\link{glmmGS.Control}} 
}
\examples{
	# Set data dimension: 
	# - 1 million observations, 
	# - 7 'dense' fixed effect covariates,
	# - stratified random effects with 2000 strata (levels)
	nobs <- 1000000L
	ndays <- 7L
	nPostCodes <- 2000L

	# Generate data
	size <- as.integer(runif(nobs, 1, 10))
	days <- matrix(rnorm(nobs * ndays), nrow = nobs, ncol = ndays)
	seifa <- rnorm(nobs)
	postCodes <- as.factor(as.integer(runif(nobs, 0, nPostCodes)))
	
	# Generate coefficients
	offset <- runif(nobs, -1, 1)
	intercept <- -0.5
	sd.days <- 0.1
	beta.days <- rnorm(ndays, sd = sd.days)
	sd.seifa <- 0.1
	intercept.seifa <- rnorm(nPostCodes, sd = sd.seifa)
	beta.seifa <- rnorm(nPostCodes, sd = sd.seifa)
	
	# Generate linear predictor
	eta <- intercept + days \%*\% beta.days
	eta <- eta + intercept.seifa[as.integer(postCodes)] + seifa * beta.seifa[as.integer(postCodes)]

	# Generate response
	y <- as.integer(rbinom(nobs, size, plogis(eta)))

	# Define identity precision model and control list
	I <- glmmGS.CovarianceModel("identity")
	control <- glmmGS.Control(reltol = 1.e-8, abstol = 1.e-25, maxit = 200)
	
	# Fit model using Gauss-Seidel algorithm with two blocks: 
	# - one fixed effect block: (1 + days);
	# - one stratified random effect block with an identity  
	#   covariance model: (1 + seifa | postCodes ~ I).
	formula <- (y / size) ~ offset(offset) + (1 + days) + (1 + seifa | postCodes ~ I)
	results <- glmmGS(formula = formula, family = binomial, control = control)
}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{Gauss}
\keyword{Seidel}
% __ONLY ONE__ keyword per line
