\name{glmmGS}
\alias{glmmGS}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{
Fit Generalized Linear Mixed Models
}
\description{
glmmGS is used to fit generalized mixed linear models specified by a string-like formula.
The MLE estimation of the parameters is found by using a Gauss-Seidel algorithm to solve
the model's normal equations.
}
\usage{
glmmGS(formula, family, data = NULL, covariance.models = NULL, control = glmmGS.Control())
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{formula}{
%%     ~~Describe \code{formula} here~~
a string providing a symbolic description of the model to be fitted.
}
  \item{family}{
%%     ~~Describe \code{family} here~~
a description of the error distribution or the link-function
}
  \item{data}{
%%     ~~Describe \code{data} here~~
an optional data frame, list or environment (or object coercible by as.data.frame to a data frame) containing the variables in the model. 
If not found in data, the variables are taken from environment(formula), typically the environment from which glmmGS is called
}
  \item{covariance.models}{
%%     ~~Describe \code{data} here~~
an optional list or environment containing the covariance models referenced in the formula. 
If not found in covariance.models, the covariance models are taken from environment(formula), 
typically the environment from which glmmGS is called
}
  \item{control}{
%%     ~~Describe \code{control} here~~
a list of parameters for controlling the fitting process
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
