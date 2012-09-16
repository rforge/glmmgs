\name{fixef}
\alias{fixef}
\title{
Extract fixed effect estimates 
}
\description{
	%%  ~~ A concise (1-5 lines) description of what the function does. ~~
	Extract the fixed effect estimates from a model fitted by the 
	\code{\link{glmmGS}} function.
}
\usage{
	fixef(object, ...)
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
	a list of numeric vectors (one vector for each block of fixed effects) of fixed effect estimates.
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
	# Run glmmGS example producing the variable 'results'
	example(glmmGS)
	
	# Extract fixed effects from 'results'
	fixef <- fixef(results)
}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ ~kwd1 }
\keyword{ ~kwd2 }
% __ONLY ONE__ keyword per line
