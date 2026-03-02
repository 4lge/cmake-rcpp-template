#include <Rcpp.h>

#include "backend/init.hpp"
// #include "backend/lib/OpenCL-Wrapper/src/opencl.hpp" // OpenCL Wrapper stuff
#include "backend/include/backend/distance_matrix.hpp"

using namespace Rcpp;

// [[Rcpp::export]]
NumericMatrix CLDistanceMatrix(const NumericMatrix& mat) {
    IntegerVector dim = mat.attr("dim");
    int rows = dim[0], cols = dim[1];

    std::vector<double> input(mat.begin(), mat.end());
    std::vector<double> output(rows * rows, 0);

    backend::distance_matrix(input, output);

    std::cout << "output[0]=" << output[0] << std::endl;
    NumericMatrix outmat(rows, rows);
    std::copy(output.begin(), output.end(), outmat.begin());

    std::cout << "outmat[0,0]=" << outmat[0,0] << std::endl;
    return outmat;
}


// You can include R code blocks in C++ files processed with sourceCpp
// (useful for testing and development). The R code will be automatically 
// run after the compilation.
//

/*** R
*/
