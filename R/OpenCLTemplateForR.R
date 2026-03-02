.onLoad <- function(libname, pkgname) {
  kernelsPath <- system.file("kernels", package = pkgname)
  print(kernelsPath)
  ## generate at least on random number to initialize .Random.seed:
  runif(1)
  .Call("InitCL", PACKAGE = pkgname)
}
