#include "backend/rnormcl.hpp"

#include "CLpp.hpp"

#include <iostream>



namespace backend {

  void EXPORT rnormcl(const int n, const double mean, const double sd, const int seed, std::vector<double>& output) { 
    // try {
    
      Device device = CLpp::instance().getActiveDevice();

      uint N=n;

      if(CLpp::instance().is_double()){
	std::cout << "double device\n";
      // copy data from host memory to device memory
      Memory<int> Seed(device, 1);
      Memory<float> Output(device, N);	
      Memory<float> Mean(device, 1);
      Memory<float> SD(device, 1);


      Seed[0]=seed;
      Mean[0] = mean;
      SD[0] = sd;

      // kernel that runs on the device
      Kernel norm_rng(device, N, "norm_rng", Output, Seed, Mean, SD); 

      // copy data from host memory to device memory
      Seed.write_to_device();
      Mean.write_to_device();
      SD.write_to_device();

      // run add_kernel on the device
      norm_rng.run(); 

      // copy data from device memory to host memory
      Output.read_from_device(); 

      for(auto i=0; i<Output.length(); i++)
        output[i] = Output[i];
	
      } else {
	std::cout << "single device\n";
      // copy data from host memory to device memory
      Memory<int> Seed(device, 1);
      Memory<float> Output(device, N);	
      Memory<float> Mean(device, 1);
      Memory<float> SD(device, 1);


      Seed[0]=seed;
      Mean[0] = mean;
      SD[0] = sd;

      // kernel that runs on the device
      Kernel norm_rng(device, N, "norm_rng", Output, Seed, Mean, SD); 

      // copy data from host memory to device memory
      Seed.write_to_device();
      Mean.write_to_device();
      SD.write_to_device();

      // run add_kernel on the device
      norm_rng.run(); 

      // copy data from device memory to host memory
      Output.read_from_device(); 

      for(auto i=0; i<Output.length(); i++)
        output[i] = Output[i];

      }
      	/*        }
        catch (cl::Error &err) {
	  throw std::runtime_error(std::string("OpenCL Error: ") + err.what() + " (" + std::to_string(err.err()) + ")\n");
        }
	  */

  }

}
