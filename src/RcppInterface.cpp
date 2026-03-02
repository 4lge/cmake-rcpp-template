#include <Rcpp.h>

#include "backend/init.hpp"

using namespace Rcpp;

/**
 * 
 *  INIT SETTER.
 * 
 *  This function takes a special place as it sets the static linkage path to the resources for C to access them.
 *  Expand this function to whatever you need to pass to the dll/so on loading.
 * 
 */
extern "C" SEXP InitCL() {
    backend::initCL();
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

static const R_CallMethodDef CallEntries[] = {
    {"InitCL", (DL_FUNC) &InitCL, 0},
    {NULL, NULL, 0}
};

extern "C" void R_init_DistanceMatrix(DllInfo* dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
