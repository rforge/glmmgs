\name{glmmGS}
\alias{glmmGS}
\title{
	Fit Generalized Linear Mixed Models
}
\description{
	glmmGS is used to fit generalized mixed linear models specified by a string-like formula.
	The estimation of the parameters is performed by fitting a Penalized Quasi Likelihood (PQL) 
	using a Gauss-Seidel algorithm to fit the regression coefficients (see Reference). 
	The algorithm is highly optimized for data sets with a large number of observations 
	and with stratified covariates with a large number of levels and diagonal covariance structures. 
	Non diagonal covariance structures defined by dense	or sparse precision 
	matrices are also allowed (see \code{\link{glmmGS.CovarianceModel}}).
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
	an object of type \code{glmmGS} containing:
	references to the input data blocks (for internal use only);   
	the estimated fixed effects to be retrieved using the generic functions \code{coef} or \code{\link{fixef}};
	the fixed effect conditional covariance matrices to be retrieved using the generic function \code{vcov}; 
	the estimated random effects to be retrieved using the generic function \code{\link{ranef}}; 
	the estimated parameters of the random effect covariance models used to calculated the variance
	components of the random effects displayed by the generic function \code{print.glmmGS};
	the number of GS iterations taken to converge.
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
	The \code{glmmGS} function is optimized to handle data-sets with very large numbers of observations 
	containing stratified data with large numbers of levels. 
	
	One of the distinguishing characteristic of the Gauss-Seidel algorithm is that the full
	precision matrix of the posterior distribution of the fixed effects
	is never computed: only the diagonal blocks associated with the input blocks are computed. 
	As a consequence, the standard errors associated with the coefficient estimates are computed 
	for each block conditionally to the other blocks, and they tend to be
	smaller than the standard-errors calculated using the full posterior
	distribution of the parameters.
	
	Although it is possible to specify as many blocks of random effects as desired, the PQL update
	of the covariance components is performed one block at a time. This yields to different
	estimates compared to a joint PQL update of the covariance components.
}

%% ~Make other sections like Warning with \section{Warning }{....} ~

\seealso{
%% ~~objects to See Also as \code{\link{help}}, ~~~
\code{\link{glmmGS.CovarianceModel}}, \code{\link{glmmGS.Control}} 
}
\examples{
	# Set data dimension: 
	# - 1 million observations, 
	# - 7 day-of-the-week specific fixed effects
	# - 2000 post-code specific random effects
	nobs <- 1000000L
	nDaysOfWeek <- 7L
	nPostCodes <- 2000L

	# Generate data
	set.seed(23)
	# Size of binomial observations
	size <- as.integer(runif(nobs, 1, 10))
	# Covariate
	x <- rnorm(nobs)
	# Fixed effects and random effects grouping factors
	dayOfWeek <- factor(as.integer(runif(nobs, 1, nDaysOfWeek + 1))) 
	postCode <- factor(as.integer(runif(nobs, 1, nPostCodes + 1)))
	
	# Set level names for dayOfWeek factor
	levels(dayOfWeek) <- c("Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday")
	
	# Generate fixed effects
	beta0 <- -0.5
	beta1 <- 0.7
	betaDays <- rnorm(nDaysOfWeek, sd = 0.1)
	betaDays[1] <- 0 # Set first level equal to zero
	
	# Generate random effects
	b0 <- rnorm(nPostCodes, sd = sqrt(0.02))
	b1 <- rnorm(nPostCodes, sd = sqrt(0.01))
	
	# Generate linear predictor
	eta <- beta0 + x * beta1
	eta <- eta + betaDays[as.integer(dayOfWeek)]
	eta <- eta + b0[as.integer(postCode)] + x * b1[as.integer(postCode)]

	# Generate response
	y <- as.integer(rbinom(nobs, size, plogis(eta)))

	# Define identity covariance model
	I <- glmmGS.CovarianceModel("identity")
	
	# Define control list
	control <- glmmGS.Control(reltol = 1.e-6, abstol = 1.e-6, maxit = 200, verbose = FALSE)
	
	# Fit model using Gauss-Seidel algorithm with three blocks: 
	# - one dense fixed effect block: (1 + x);
	# - one stratified fixed effect block: (1 | dayOfWeek);
	# - one stratified random effect block: (1 + x | postCode ~ I)
	#   with an identity prior covariance matrix
	result1 <- glmmGS((y / size) ~ (1 + x) + (1 | dayOfWeek) + (1 + x | postCode ~ I), 
		family = binomial, control = control)
	
	# Refit model supplying covariance components
	Ic <- glmmGS.CovarianceModel("identity", fix.components = cov(cbind(b0, b1)))
	result2 <- glmmGS((y / size) ~ (1 + x) + (1 | dayOfWeek) + (1 + x | postCode ~ Ic), 
		family = binomial, control = control)
	
	# Print results
	print(result1)
	print(result2)
}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{Gauss}
\keyword{Seidel}
% __ONLY ONE__ keyword per line
