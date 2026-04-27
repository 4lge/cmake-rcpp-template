
ActivateDeviceWithMostMemory <- function() .Call("ActivateDeviceWithMostMemory")

ActivateDeviceWithMostFlops <- function() .Call("ActivateDeviceWithMostFlops")

ActivateDeviceWithID <- function(id) .Call("ActivateDeviceWithID", as.integer(id))

InitCL <- function() .Call("InitCL")

SetKernelsPath <- function(path) .Call("SetKernelsPath", as.character(path))
