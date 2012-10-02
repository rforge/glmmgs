\name{glmmGS.CovarianceModel}
\alias{glmmGS.CovarianceModel}
\title{
	Construct covariance models for the \code{glmmGS} function
}
\description{
	%%  ~~ A concise (1-5 lines) description of what the function does. ~~
	Construct a list defining a covariance model for the \code{\link{glmmGS}} function. 
}

%% Function prototype
\usage{
	glmmGS.CovarianceModel(type, \dots)
}

%% Arguments
\arguments{
  	\item{type}{
		a string specifying the type of the covariance model. Current valid types are:
		\code{identity} and \code{precision}. The covariance components of the identity
		and precision models represent the global precision parameters.
  	}
  	\item{\dots}{
    	additional arguments containing information about the precision or covariance structure.
    	If the type is \code{identity}, no additional arguments are required;
    	if the type is \code{precision}, the required additional argument must be 
    	a precision matrix of either R \code{matrix} type or \code{\link{glmmGS.SparseMatrix}} type.
    	If the covariance model is applied to random effect covariates stratified by
    	a grouping factor, then a within-stratum covariance model for the random effects
    	can be further specified by the named argument \code{within}. In particular,
    	\code{within = "diagonal"} fits a within-stratum diagonal precision matrix, 
    	while \code{within = "multivariate"} fits a within-stratum complete precision matrix.
    	By default a diagonal within-stratum precision matrix is assumed.
    	The covariance components can be fixed by passing the vector
    	of parameters used by the covariance model in the named argument \code{fix.components}.
    	For stratified random effects the vector of parameters depends on: 
    	the covariance model type, the number of random effect covariates, 
    	and the value of the \code{within} argument.
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
	A list of parameters specifying a covariance model for the \code{\link{glmmGS}} function.
}
\references{
	%% ~put references to the literature/web site here ~
}
\author{
	%%  ~~who you are~~
	Michele Morara, Louise Ryan, Subharup Guha, Christopher Paciorek
}
\note{
	%%  ~~further notes~~
}

%% ~Make other sections like Warning with \section{Warning }{....} ~

\seealso{
	%% ~~objects to See Also as \code{\link{help}}, ~~~
	\code{\link{glmmGS}}, \code{\link{glmmGS.SparseMatrix}}
}
\examples{
# Set dimensions of hypothetical data set
	
	nlevels <- 100 	# Number of levels of a hypothetical stratified data set
	nvars <- 2		# Number of random effect covariates within each stratum

# Identity covariance model (no need to specify matrix dimensions)
 	
 	# Define dense-precision model
 	I <- glmmGS.CovarianceModel("identity")

# Dense-precision model

	# Dense precision matrix
	ncols <- nlevels
	R <- matrix(rnorm(ncols * ncols), nrow = ncols, ncol = ncols)
	R <- R \%*\% t(R)
	
 	# Define dense-precision model
 	T.dense <- glmmGS.CovarianceModel("precision", R)

# Sparse-precision model

	# Sparse precision matrix
	ncols <- nlevels
	R <- diag(rep(1, ncols))
	for (i in 2:ncols)
	{
		R[i - 1, i] <- 0.5
		R[i, i - 1] <- 0.5
	}
	
	# Create sparse matrix from R
	R.sparse = glmmGS.SparseMatrix(R)
	
 	# Define sparse-precision model
 	T.sparse <- glmmGS.CovarianceModel("precision", R.sparse)

# Identity model with supplied covariance components

	# Set a vector of precision parameters to be used with two correlated random effect variables
	# associated with stratified covariates
	within.cov <- matrix(c(1.0, 0.1, 0.1, 2.0), nrow = 2L, ncol = 2L)
	within.prec <- solve(within.cov)
	true.theta <- as.vector(within.prec[lower.tri(within.prec, diag = TRUE)])
		
 	# Define sparse-precision model and supplies covariance components
 	I.mv.fixed <- glmmGS.CovarianceModel("identity", within = "multivariate", fix.components = true.theta)
}

% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ ~kwd1 }
\keyword{ ~kwd2 }
% __ONLY ONE__ keyword per line
