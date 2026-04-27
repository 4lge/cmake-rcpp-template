#include <Rcpp.h>

#include "backend/init.hpp"
// #include "backend/lib/OpenCL-Wrapper/src/opencl.hpp" // OpenCL Wrapper stuff
#include "backend/include/backend/rnormcl.hpp"

using namespace Rcpp;

// [[Rcpp::export]]
NumericVector rnormCL(const int n, const double mean, const double sd) {
  
  Environment g = Environment::global_env() ;
  //Environment::Binding RandomSeed = g[".Random.seed"] ;
  //IntegerVector randomSeed = RandomSeed;
  IntegerVector randomSeed = g[".Random.seed"] ;
  
  // extract .Random.seed[.Random.seed[2]+2]
  int seed=randomSeed[randomSeed[1]+1];
  
  //std::cout << "seed=" << seed << std::endl;
  
  std::vector<double> output(n);
  
  backend::rnormcl(n, mean, sd, seed, output);

  //std::cout << "output[1]=" << output[0] << std::endl;
  
  NumericVector res(n);
  std::copy(output.begin(), output.end(), res.begin());

  //std::cout << "res[1]=" << res[0] << std::endl;

  // waste a single random number in R to update .Random.seed
  Function f("runif");
  SEXP r=f(1);
  
  return res;
}


// You can include R code blocks in C++ files processed with sourceCpp
// (useful for testing and development). The R code will be automatically 
// run after the compilation.
//

/*** R
*/
