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
		\code{identity}, \code{precision}. The covariance components of the identity
		and precision models represent the global precision parameters.
  	}
  	\item{\dots}{
    	additional arguments containing information about the precision or covariance structure.
    	If the type is \code{identity}, no additional arguments are required;
    	if the type is \code{precision}, the required additional argument must be 
    	a precision matrix of either R \code{matrix} type or \code{\link{glmmGS.SparseMatrix}} type.
    	In all covariance models the covariance components can be fixed by setting
    	the named argument \code{fix.components} equal to a given covariance
    	component matrix.
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

# Sparse-precision model with supplied covariance components

	# Define a covariance component matrix of dimensions equal to the number of random effect covariates 
	S <- matrix(c(0.1, 0, 0, 0.2), nrow = nvars, ncol = nvars) 
	
 	# Define sparse-precision model and supplies covariance components
 	Tc.sparse <- glmmGS.CovarianceModel("precision", R.sparse, fix.components = S)
}

% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ ~kwd1 }
\keyword{ ~kwd2 }
% __ONLY ONE__ keyword per line
