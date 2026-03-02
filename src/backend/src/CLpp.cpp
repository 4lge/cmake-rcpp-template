#include "CLpp.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>



namespace backend {

  CLpp::CLpp() {
    this->activeDevice = Device(select_device_with_most_flops()); // compile OpenCL C code for the fastest available device
    //this->activeDevice = Device(select_device_with_most_memory()); // compile OpenCL C code for the device with most RAM
    //this->activeDevice = Device(select_device_with_id(1)); // compile OpenCL C code for the fastest available device

    /*
	cl_device_fp_config fp_config = device.getInfo<CL_DEVICE_DOUBLE_FP_CONFIG>();
	if(fp_config & (CL_FP_FMA | CL_FP_ROUND_TO_NEAREST | CL_FP_ROUND_TO_ZERO | CL_FP_ROUND_TO_INF | CL_FP_INF_NAN | CL_FP_DENORM)){
	  std::cout << "Device is double capable" << "\n";
	} else {
	  std::cout << "Device is float only" << "\n";
	}
	*/
  }

  void CLpp::info() {
    std::cout << "name: " << this->activeDevice.info.name << std::endl;
  }
  
  void CLpp::activate(int platform_id, int device_id){
  }
  
  Device CLpp::getActiveDevice(){
    return this->activeDevice;
  }
  
  CLpp::~CLpp() {}

  CLpp& CLpp::instance() {
    static CLpp instance;
    return instance;
  }
  

}
