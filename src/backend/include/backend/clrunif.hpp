#pragma once

#include "backend/export.hpp"
#include <vector>
#include <string>

namespace backend {

	/**
	 * 	FUNCTION TO CALCULATE UNIFORM RANDOM NUMBERS
	 * 
	 * 	lower, upper are the parameters, seed is provided externally
	 * 	result is a 1d array of length n
	 */
  
  void clrunif(const int n, const double lower, const double upper, const int seed, std::vector<double>& output);
  
}
