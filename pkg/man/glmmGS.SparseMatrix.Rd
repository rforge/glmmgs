\name{glmmGS.SparseMatrix}
\alias{glmmGS.SparseMatrix}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{
%%  ~~function to do ... ~~
Construct sparse matrix for glmmGS
}
\description{
%%  ~~ A concise (1-5 lines) description of what the function does. ~~
}
\usage{
glmmGS.SparseMatrix(...)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{...}{
	%%     ~~Describe \code{reltol} here~~
	Can be a dense matrix or the following arguments in order: 
	1) number of columns; 
	2) a vector with the non-zero elements of the matrix;
	3) a vector with the zero-based row indices of the non-zero elements of the matrix;
	4) a vector of length ncols + 1, such that counts[1] = 0, and counts[j] equal to the total number  
	of non-zero elements in columns 1, ..., j - 1.
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
}
\references{
%% ~put references to the literature/web site here ~
}
\author{
%%  ~~who you are~~
}
\note{
%%  ~~further notes~~
}

%% ~Make other sections like Warning with \section{Warning }{....} ~

\seealso{
%% ~~objects to See Also as \code{\link{help}}, ~~~
}
\examples{
##---- Should be DIRECTLY executable !! ----
}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ ~kwd1 }
\keyword{ ~kwd2 }% __ONLY ONE__ keyword per line
