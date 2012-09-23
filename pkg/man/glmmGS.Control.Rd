\name{glmmGS.Control}
\alias{glmmGS.Control}
\title{
Construct control list for \code{glmmGS} function
}
\description{
	%%  ~~ A concise (1-5 lines) description of what the function does. ~~
	Construct a list of control parameters used internally by the 
	\code{\link{glmmGS}} function.
}
\usage{
	glmmGS.Control(
		reltol = 1e-06, 
		abstol = 1e-06, 
		maxit = list(200L, 200L), 
		verbose = FALSE,
		max.update = c(0, 0, 0),
		max.value = c(0, 0, 0))
}
%- maybe also 'usage' for other objects documented here.
\arguments{
 	\item{reltol}{
		relative tolerance for the parameter update.
	}
 	\item{abstol}{
		absolute tolerance for the parameter update.
	}
  	\item{maxit}{
		maximum number of outer and inner iterations for the parameter update
		(see Details). If only one number is provided, then the number of outer and inner 
		iterations is assumed to be identical.
	}
  	\item{verbose}{
		logical scalar. If TRUE, then verbose output is generated during the 
		estimation of the parameters.
	}
	\item{max.update}{
		(advanced option) when different from \code{0} sets the maximum absolute 
		update for fixed effects, random effects, and variance components respectively. 
	}
	\item{max.value}{
		(advanced option) when different from \code{0} sets the maximum absolute value 
		for fixed effects, random effects, and variance components respectively. 
		Only the bound of the variance components is used; 
		the bounds of the fixed and random effects have been included 
		for future compatibility.
	}
}
\details{
	%%  ~~ If necessary, more details than the description above ~~
	The Gauss-Seidel iterative algorithm stops when the magnitude of every 
	parameter update is less than \code{abstol} or less than \code{reltol} 
	times the magnitude of the current parameter.
	The \emph{inner iterations} are consecutive updates 
	of the regression coefficients (fixed and random effects) assuming 
	the variance components fixed and vice versa. 
	The \emph{outer iterations} are alternate updates 
	of coefficients and variance components respectively.
	If the algorithm reaches \code{maxit$inner} number of inner iterations without
	converging, then it moves on to update the next set of parameters; 
	if it reaches \code{maxit$outer} number	of outer iterations without converging, 
	then it stops and returns an error.
}

\value{
	%%  ~Describe the value returned
	%%  If it is a LIST, use
	%%  \item{comp1 }{Description of 'comp1'}
	%%  \item{comp2 }{Description of 'comp2'}
	%% ...
	A list of control parameters for the \code{\link{glmmGS}} function.
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
	\code{\link{glmmGS}}
}

\examples{
	control <- glmmGS.Control(reltol = 1.e-8, abstol = 1.e-25, maxit = 500);
}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ ~kwd1 }
\keyword{ ~kwd2 }
% __ONLY ONE__ keyword per line
