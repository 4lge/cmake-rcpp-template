\name{CLDistanceMatrix}
\alias{CLDistanceMatrix1}
\alias{CLDistanceMatrix2}
\title{Distance Matrix Calculation via OpenCL}
\description{
    Given a tabular data set X calculates its distance matrix D, i.e.
    D[i,j] = ||X[i,] - X[j,]||
    via an OpenCL backend in C++.
}
\usage{
CLDistanceMatrix1(mat)
CLDistanceMatrix2(mat)
}
\arguments{
    \item{mat}{A tabular dataset as a matrix}
}
\value{
  \item{D}{Distance Matrix of X.}
}
\author{Julian Märte}
\note{
	variant 1 is the original version (using low level CL code),
	variant 2 is modified to use OpenCL-Wrapper (alge@algepop.net)
	and uses C++ abstraction to hide low level CL coding.
}
\keyword{ distance, opencl }
