#include "backend/clrunif.hpp"

#include "CLpp.hpp"

#include <iostream>



namespace backend {

  void clrunif(const int n, const double lower, const double upper, const int seed, std::vector<double>& output) { 
    // try {
    
      Device device = CLpp::instance().getActiveDevice();

      uint N=n;
      
      // copy data from host memory to device memory
      Memory<int> Seed(device, 1);
      Memory<float> Output(device, N);	
      Memory<float> Lower(device, 1);
      Memory<float> Upper(device, 1);


      Seed[0]=seed;
      Lower[0] = lower;
      Upper[0] = upper;

      // kernel that runs on the device
      Kernel unif_rng(device, N, "unif_rng", Output, Seed, Lower, Upper); 

      // copy data from host memory to device memory
      Seed.write_to_device();
      Lower.write_to_device();
      Upper.write_to_device();

      // run add_kernel on the device
      unif_rng.run(); 

      // copy data from device memory to host memory
      Output.read_from_device(); 

      std::cout << "Output[1]=" << Output[0] << std::endl;

      for(auto i=0; i<Output.length(); i++)
        output[i] = Output[i];

      std::cout << "output[1]=" << output[0] << std::endl;

	/*        }
        catch (cl::Error &err) {
	  throw std::runtime_error(std::string("OpenCL Error: ") + err.what() + " (" + std::to_string(err.err()) + ")\n");
        }
	  */
      
  }

}
