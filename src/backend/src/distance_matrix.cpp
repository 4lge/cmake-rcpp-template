#include "backend/distance_matrix.hpp"

#include "CLpp.hpp"
#include "CL_context.hpp"

#include <iostream>



namespace backend {

  void distance_matrix(std::vector<double> data, std::vector<double> res) { // (double* data, int rows, int cols, double* res) {
    // try {

	  Device device = CLpp::instance().getActiveDevice();
	  const uint N = data.size();// 256u; // size of vectors
	  const uint M = res.size();// 256u; // size of vectors
	  Memory<float> Input(device, N); // allocate memory on both host and device
	  Memory<int> NN(device, 1); // allocate memory on both host and device
	  Memory<float> Output(device, M);
	  Memory<int> MM(device, 1); // allocate memory on both host and device

	  
	  Kernel distance_matrix_kernel(device, N, "distance_matrix", Input, Output, NN, MM); // kernel that runs on the device

	  for(auto i=0; i<N; i++)
	    Input[i] = data[i];
	  NN[0]=N; MM[0]=M;
    
	  Input.write_to_device(); // copy data from host memory to device memory
	  NN.write_to_device(); // copy data from host memory to device memory
	  MM.write_to_device(); // copy data from host memory to device memory
	  distance_matrix_kernel.run(); // run add_kernel on the device
	  Output.read_from_device(); // copy data from device memory to host memory

	  for(auto i=0; i<M; i++)
	    res[i] = Output[i];
    
	  /*        }
        catch (cl::Error &err) {
	  throw std::runtime_error(std::string("OpenCL Error: ") + err.what() + " (" + std::to_string(err.err()) + ")\n");
        }
	  */
	
  }
}
