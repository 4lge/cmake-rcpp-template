#pragma once

#include "backend/export.hpp"
#include <vector>
#include <string>

namespace backend {

	/**
	 * 	FUNCTION TO CALCULATE THE DISTANCE MATRIX OF data AND WRITE IT INTO res.
	 * 
	 * 	data is a 1d array of length rows * cols.
	 * 	res is a 1d array of length rows * rows.
	 */
  
  void distance_matrix(std::vector<double> data, std::vector<double> res);

}