\name{ranef}
\alias{ranef}
\title{
Extract random effect estimates 
}
\description{
	%%  ~~ A concise (1-5 lines) description of what the function does. ~~
	Extract the random effect estimates from a model fitted by the 
	\code{\link{glmmGS}} function.
}
\usage{
	ranef(object, ...)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
 	\item{object}{
		an object returned by the \code{\link{glmmGS}} function.
	}
	\item{...}{
		optional additional arguments. Currently non are used.
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
	a list of numeric vectors or matrix (one vector or matrix for each block of random effects) 
	of random effect estimates. If, in a given block, the covariates associated with the random effects are stratified, then 
	the random effect estimates are returned as a matrix with one column for each covariate and
	as many rows as many strata. Otherwise the random effect block estimates are returned as vectors. 
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
}

\examples{
	# Run glmmGS example producing the variable 'result1' and 'result2'
	example(glmmGS)
	
	# Extract random effect estimates from results 
	ranef1 <- ranef(result1)
	ranef2 <- ranef(result2)
	
	# Print covariance of estimated random effects in first (and only) 
	# random effect block: '1 + x | postCode ~ I', '1 + x | postCode ~ Ic' 
	# (see glmmGS example)
	print(cov(cbind(ranef1[[1]][, 1], ranef1[[1]][, 2])))
	print(cov(cbind(ranef2[[1]][, 1], ranef2[[1]][, 2])))
}

% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ ~kwd1 }
\keyword{ ~kwd2 }
% __ONLY ONE__ keyword per line
