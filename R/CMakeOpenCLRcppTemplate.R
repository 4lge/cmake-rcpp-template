# need this hack to mention C EXPORT functions to be 
# included in Rcpp::compileAttributes() run
.fakeLoad <- function(libname, pkgname) {
  .Call("ActivateDeviceWithID", 0, PACKAGE = pkgname)
  .Call("ActivateDeviceWithMostMemory", PACKAGE = pkgname)
  .Call("ActivateDeviceWithMostFlops", PACKAGE = pkgname)
}

# real onLoad function:
.onLoad <- function(libname, pkgname) {
  kernelsPath <- system.file("kernels", package = pkgname)
  print(kernelsPath)
  .Call("SetKernelsPath", kernelsPath, PACKAGE = pkgname)
  .Call("ActivateDeviceWithMostFlops", PACKAGE = pkgname)
  ## generate at least on random number to initialize .Random.seed:
  runif(1)
  .Call("InitCL", PACKAGE = pkgname)
}
