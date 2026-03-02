#pragma once


#include "opencl.hpp" // OpenCL Wrapper stuff


#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_TARGET_OPENCL_VERSION 210
#define CL_HPP_TARGET_OPENCL_VERSION 210
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#include <CL/opencl.hpp>

#include <string>
#include <mutex>

#include "cl_error_lookup.hpp"

namespace backend {


  /**
   * 	A singleton class managing the external library state (in this case OpenCL).
   * 	
   *   interface to OpenCL-Wrapper
   *
   */
  class CLpp {
  public:
    // gets the static linkage CL state
    static CLpp& instance();


    void activate(int platform_id, int device_id);
    
    Device getActiveDevice();
    
    void info();

    void initCL();
    
    // Build a program from a kernel file
    cl::Program get_program(const std::string& filepath) const;

  private:
    // Initializer runs the typical init code for your external library
    CLpp();
    // Destructor cleans up the external library state
    ~CLpp();

    Device activeDevice;
    
    // thread safe singleton
    CLpp(const CLpp&) = delete;
    CLpp& operator=(const CLpp&) = delete;


    // CL constants:
    /*
    std::vector<cl::Platform> platforms;
    std::vector<cl::Device> devices;    
    cl::Device best_device;
    cl::Platform best_platform;
    cl::Device active_device;
    cl::Platform active_platform;
    cl::Context context;
    cl::CommandQueue queue;

    // attached resource path
    std::string kernels_path;
    */
    // thread safety
    mutable std::mutex mutex;
  };

}
