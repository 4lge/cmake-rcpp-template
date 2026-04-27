#include <Rcpp.h>

#include "backend/init.hpp"
// #include "backend/lib/OpenCL-Wrapper/src/opencl.hpp" // OpenCL Wrapper stuff
#include "backend/include/backend/runifcl.hpp"

using namespace Rcpp;

// [[Rcpp::export]]
NumericVector runifCL(const int n, const double lower, const double upper) {
  
  Environment g = Environment::global_env() ;
  //Environment::Binding RandomSeed = g[".Random.seed"] ;
  //IntegerVector randomSeed = RandomSeed;
  IntegerVector randomSeed = g[".Random.seed"] ;
  
  // extract .Random.seed[.Random.seed[2]+2]
  int seed=randomSeed[randomSeed[1]+1];
  
  std::cout << "seed=" << seed << std::endl;
  
  std::vector<double> output(n);
  
  backend::runifcl(n, lower, upper, seed, output);

  std::cout << "output[1]=" << output[0] << std::endl;
  
  NumericVector res(n);
  std::copy(output.begin(), output.end(), res.begin());

  std::cout << "res[1]=" << res[0] << std::endl;

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
