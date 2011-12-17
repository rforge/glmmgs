\name{glmmGS}
\alias{glmmGS}
\title{
	Fit Generalized Linear Mixed Models
}
\description{
	glmmGS is used to fit generalized mixed linear models specified by a string-like formula.
	The MLE estimation of the parameters is performed by using a Gauss-Seidel algorithm 
	to fit the regression coefficients (see reference), and a a Penalized Quasi Likelihood 
	approach to fit the covariance components. The algorithm is highly optimized to fit 
	fixed-effect models to data-sets with a large number of observations and 
	with stratified covariates with a large number of levels, and mixed model 
	with stratified covariates with a large number of levels and diagonal
	covariance structures. Covariance structures defined by non diagonal dense 
	or sparse precision matrices can also allowed (see \code{\link{glmmGS.CovarianceModel}}).
}
\usage{
	glmmGS(formula, family, data = NULL, covariance.models = NULL, control = glmmGS.Control(), random.effects.covariance = FALSE)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
	\item{formula}{
		a formula object providing a symbolic description of the model to be fitted.
		The formula object must be of the type \emph{response ~ linear-predictor}. For a binomial
		response, the response must be defined as \code{(outcome | counts)}. The linear predictor 
		comprises fixed-effects and random-effects. Fixed and random effects must be grouped 
		in blocks (the blocks of the Gauss-Seidel algorithm, see reference) 
		specified by pairs of parentheses. Blocks of covariates can be stratified by a grouping
		variable. Only one block of random effects can be currently specified.
		The covariance model inside the random effect block is specified by using the symbol 
		\code{~} followed by a variable-name of a previously defined \code{\link{glmmGS.CovarianceModel}}).
		In an offset \code{a} is present in the linear predictor, it must be specified with keyword
		\code{offset(a)} before the specification of the fixed and random effects. 
		In order to maximize the memory usage, non of the variables present in the formula
		are copied or coerced. In particular:
		discrete outcomes must be integer vectors;
		covariates must be vectors of either integer or double type; 
		grouping variables used to specify stratified covariates must be integer 
		vectors containing zero-based consecutive levels.
	}
	\item{family}{
		a description of the error distribution. Only two family are currently allowed:
		\code{binomial} and \code{poisson} with a canonical link.
	}
	\item{data}{
		an optional data frame, list or environment (or object coercible by as.data.frame 
		to a data frame) containing the variables in the model. 
		If not found in data, the variables are taken from environment(formula), 
		typically the environment from which \code{glmmGS} is called.
	}
	\item{covariance.models}{
		an optional list or environment containing the covariance models referenced 
		in the formula. If not found in covariance.models, the covariance models are taken from 
		environment(formula), typically the environment from which \code{glmmGS} is called.
	}
	\item{control}{
		a list of parameters for controlling the fitting process. The list is returned
		by the \code{\link{glmmGS.Control}} function.
	}
	\item{random.effects.covariance}{
		a boolean variable specifying whether or not the full covariance matrix of the random effects
		should be returned.	
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
	The function returns a list of values comprised of
	\item{fixed.effects}{
		a list containing the items:
		\code{estimates}: the estimates of the fixed effect coefficients;
		\code{stderrs}: the standard errors of the fixed effect estimates;
		\code{covariance}, a list of conditional covariance matrices 
		for each fixed effect block specified in the formula.
	}
	\item{covariance.components}{
		a list containing the items:
		\code{estimates}: the estimates of the covariance components;
		\code{stderrs}: the standard errors of the covariance component estimates;
		\code{covariance}: the covariance matrice of the covariance component estimates.
	}
	\item{random.effects}{
		a list containing the items:
		\code{estimates}: the estimates of the random effect coefficients;
		\code{stderrs}: the standard errors of the random effect estimates;
		\code{covariance}, (optional) the conditional covariance matrix 
		of the distribution of the random effects.
	}
	\item{iterations}{
		the number of iterations.
	}
	\item{fitted.values}{
		the fitted values.
	}
}

\references{
	Guha, S., Ryan, L., Morara M. (2009) Gauss-Seidel Estimation of Generalized Linear
	Mixed Models With Application to Poisson Modeling of Spatially Varying Disease Rates. 
	\emph{Journal of Computational and Graphical Statistics}, \bold{4}, 810--837.
}
\author{
	Michele Morara, Luise Ryan, Subarhu Guha, Chris Paciorek  
}
\note{
%%  ~~further notes~~
}

%% ~Make other sections like Warning with \section{Warning }{....} ~

\seealso{
%% ~~objects to See Also as \code{\link{help}}, ~~~
}
\examples{
	# Set data dimension: 
	# - 1 million observations, 
	# - 7 'dense' fixed effect covariates,
	# - stratified random effects with 2000 strata (levels)
	nobs <- 1000000;
	ndays <- 7;
	nPostCodes <- 2000;

	# Generate data
	counts <- as.integer(runif(nobs, 1, 10));
	days <- matrix(rnorm(nobs * ndays), nrow = nobs, ncol = ndays);
	seifa <- rnorm(nobs);
	postCodes <- as.factor(as.integer(runif(nobs, 0, nPostCodes)));
	ipostCodes <- as.integer(postCodes) - 1L; # zero-based vector of indices
	
	# Generate coefficients
	offset = runif(nobs, -1, 1);
	intercept = -0.5;
	sd.days <- 0.1;
	beta.days <- rnorm(ndays, sd = sd.days);
	sd.seifa <- 0.1;
	intercept.seifa <- rnorm(nPostCodes, sd = sd.seifa);
	beta.seifa <- rnorm(nPostCodes, sd = sd.seifa);
	
	# Generate linear predictor
	eta <- intercept + days \%*\% beta.days + intercept.seifa[as.integer(postCodes)] + seifa * beta.seifa[as.integer(postCodes)];

	# Generate response
	y <- as.integer(rbinom(nobs, counts, plogis(eta)));

	# Define identity precision model and control list
	I <- glmmGS.CovarianceModel("identity");
	control <- glmmGS.Control(reltol = 1.e-8, abstol = 1.e-25, maxit = 500);
	
	# Fit model using Gauss-Seidel algorithm with two blocks: 
	# - one fixed effect block: (1 + days);
	# - one stratified random effect block with an identity  
	#   covariance model: (1 + seifa | ipostCodes ~ I).
	results <- glmmGS(formula = (y | counts) ~ offset(offset) + (1 + days) + (1 + seifa | ipostCodes ~ I), family = binomial, control = control);
}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{Gauss}
\keyword{Seidel}% __ONLY ONE__ keyword per line