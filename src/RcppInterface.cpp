#include <Rcpp.h>

#include "backend/init.hpp"

using namespace Rcpp;

/**
 * 
 *  SEXP WRAPPER SECTION.
 * 
 *  SEXP wrappers are functions whose sole purpose is redirection of inputs and outputs to and from Rcpp functions.
 * 
 */
extern "C" SEXP ActivateDeviceWithMostFlops() {
    backend::activateDeviceWithMostFlops();
    return R_NilValue;
}

extern "C" SEXP ActivateDeviceWithMostMemory() {
    backend::activateDeviceWithMostMemory();
    return R_NilValue;
}

extern "C" SEXP ActivateDeviceWithID(SEXP id) {
  backend::activateDeviceWithID(Rcpp::as<int>(id));
    return R_NilValue;
}

extern "C" SEXP InitCL() {
    backend::initCL();
    return R_NilValue;
}

extern "C" SEXP SetKernelsPath(SEXP kernelsPathSEXP) {
    std::string path = as<std::string>(kernelsPathSEXP);
    backend::setKernelsPath(path);
    return R_NilValue;
}

/**
 * 
 *  INIT SECTION.
 * 
 *  When wrapping your functions via SEXP manually you have to explicitly tell Rcpp how
 *  many SEXP arguments your wrapper functions have and register them under an export name.
 * 
 */
/*
static const R_CallMethodDef CallEntries[] = {
    {"ActivateDeviceWithMostFlops", (DL_FUNC) &ActivateDeviceWithMostFlops, 0},
    {"ActivateDeviceWithMostMemory", (DL_FUNC) &ActivateDeviceWithMostMemory, 0},
    {"ActivateDeviceWithID", (DL_FUNC) &ActivateDeviceWithID, 1},
    {"SetKernelsPath", (DL_FUNC) &SetKernelsPath, 1},
    {"InitCL", (DL_FUNC) &InitCL, 0},
    {NULL, NULL, 0}
};
*/

