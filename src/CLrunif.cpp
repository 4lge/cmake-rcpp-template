#include <Rcpp.h>

#include "backend/init.hpp"
// #include "backend/lib/OpenCL-Wrapper/src/opencl.hpp" // OpenCL Wrapper stuff
#include "backend/include/backend/clrunif.hpp"

using namespace Rcpp;

// [[Rcpp::export]]
NumericVector runifCL(const int n, const double lower, const double upper) {
  
  Environment g = Environment::global_env() ;
  Environment::Binding RandomSeed = g[".Random.seed"] ;
  IntegerVector randomSeed = RandomSeed;
  
  // extract .Random.seed[.Random.seed[2]+2]
  int seed=randomSeed[randomSeed[1]+2];
  
  std::cout << "seed=" << seed << std::endl;
  
  std::vector<double> output(n);
  
  backend::clrunif(n, lower, upper, seed, output);

  std::cout << "output[1]=" << output[0] << std::endl;
  
  NumericVector res(n);
  std::copy(output.begin(), output.end(), res.begin());

  std::cout << "res[1]=" << res[0] << std::endl;
  /*
  if(randomSeed[1]<624){
    randomSeed[1]=randomSeed[1]+1;
  } else {
    // Twist:
    for (int i = 0; i < 624; ++i) {
      uint temp = randomSeed[i+2] ^ (randomSeed[(i + 1) % 624+2] >> 1);
      temp ^= (temp >> 1) ^ ((temp & 1) ? 0x8EB8B000 : 0);
      randomSeed[i+2] = randomSeed[((i + 397) % 624) + 2] ^ temp;
    }
    randomSeed[1]=1;
    // FIXME: randomSeed[0]??
  }
  */
  return res;
}


// You can include R code blocks in C++ files processed with sourceCpp
// (useful for testing and development). The R code will be automatically 
// run after the compilation.
//

/*** R
*/
