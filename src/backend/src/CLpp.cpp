#include "CLpp.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>



namespace backend {

  CLpp::CLpp() {
    //this->activeDevice = Device(select_device_with_most_flops()); // compile OpenCL C code for the fastest available device
   
    this->activeDevice = Device(select_device_with_most_memory(), this->collect_opencl_c_code()); // compile OpenCL C code for the device with most RAM  this->activeDevice = Device(select_device_with_most_memory(), this->collect_opencl_c_code()); // compile OpenCL C code for the device with most RAM
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
    std::cout << "fp64: " << this->activeDevice.info.is_fp64_capable << std::endl;
    
  }

  void CLpp::activateDevice(Device_Info dev){
    this->activeDevice = Device(dev, this->collect_opencl_c_code());
  }
  void CLpp::activateDeviceWithMostFlops(){
    this->activeDevice = Device(select_device_with_most_flops(), this->collect_opencl_c_code());
  }
  void CLpp::activateDeviceWithMostMemory(){
    this->activeDevice = Device(select_device_with_most_memory(), this->collect_opencl_c_code());
  }
  void CLpp::activateDeviceWithID(int id){
    this->activeDevice = Device(select_device_with_id(id), this->collect_opencl_c_code());
  }
  
  string CLpp::collect_opencl_c_code(){
    string opencl_c_lib = get_opencl_c_code();
    return opencl_c_lib;
  }
  
  bool CLpp::is_double(){
    return activeDevice.info.is_fp64_capable>=1;
  }
  
  void CLpp::activate(int platform_id, int device_id){
  }
  
  Device CLpp::getActiveDevice(){
    return this->activeDevice;
  }

  void CLpp::setKernelsPath(const std::string& path) {
    std::lock_guard<std::mutex> lock(mutex);
    kernels_path = path;
  }
  
  std::string CLpp::getKernelsPath() const {
    std::lock_guard<std::mutex> lock(mutex);
    return kernels_path;
  }
  
  CLpp::~CLpp() {}

  CLpp& CLpp::instance() {
    static CLpp instance;
    return instance;
  }
  
}
