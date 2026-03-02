#include "CL_context.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>


namespace backend {

	CL_context::CL_context() {
        	try {
		    cl::Platform::get(&platforms);
		    if (platforms.empty()) {
		        throw std::runtime_error("No OpenCL platforms found.");
		    }
		} catch (cl::Error &e) {
			std::cerr << "*** IMPORTANT: No OpenCL platforms where found! Make sure you have a graphics driver installed for your hardware! ***" << std::endl;
			std::cerr << "*** HINT: NVIDIA Drivers might not be loaded even though they were installed due to 'Secure Boot' being enabled ***" << std::endl;
		    throw std::runtime_error("OpenCL error: " + std::string(e.what()) + " (" + std::to_string(e.err()) + ")");
		}

		bool found = false;
    int platform_cnt = 0;
		for (const auto& platform : platforms) {
      std::cout << "\tPlatform no: " << platform_cnt << std::endl;
      std::cout << "\tPlatform name: " << platform.getInfo<CL_PLATFORM_NAME>() << std::endl;
      std::cout << "\tPlatform vendor: " << platform.getInfo<CL_PLATFORM_VENDOR>() << std::endl;
      std::cout << "\tPlatform version: " << platform.getInfo<CL_PLATFORM_VERSION>() << std::endl;
      std::cout << "\tPlatform profile: " << platform.getInfo<CL_PLATFORM_PROFILE>() << std::endl;
      std::cout << "\tPlatform extensions: " << platform.getInfo<CL_PLATFORM_EXTENSIONS>() << std::endl;
      
 
		    platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

      int device_cnt = 0;
		    for (const auto& dev : devices) {
		        cl_device_type type = dev.getInfo<CL_DEVICE_TYPE>();
		        cl_uint compute_units = dev.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
		        std::string name = dev.getInfo<CL_DEVICE_NAME>();
		        std::string vendor = dev.getInfo<CL_DEVICE_VENDOR>();
	std::cout << "\tDevice no: " << device_cnt << std::endl;
	// 
	// 
	cl_device_fp_config fp_config = dev.getInfo<CL_DEVICE_DOUBLE_FP_CONFIG>();
	if(fp_config & (CL_FP_FMA | CL_FP_ROUND_TO_NEAREST | CL_FP_ROUND_TO_ZERO | CL_FP_ROUND_TO_INF | CL_FP_INF_NAN | CL_FP_DENORM)){
	  std::cout << "Device is double capable" << "\n";
	} else {
	  std::cout << "Device is float only" << "\n";
	}

	std::cout << "Found device: " << name << " (" << vendor << ") at plat.dev:" << platform_cnt << "." << device_cnt 
		  << "\n | Type: " << (type == CL_DEVICE_TYPE_GPU ? "GPU" :
		                                     type == CL_DEVICE_TYPE_CPU ? "CPU" :
		                                     type == CL_DEVICE_TYPE_ACCELERATOR ? "Accelerator" : "Other")
		  << "\n | Compute Units: " << compute_units << "\n";

		        // Prioritize GPU over others
		        if (!found || (type == CL_DEVICE_TYPE_GPU && best_device.getInfo<CL_DEVICE_TYPE>() != CL_DEVICE_TYPE_GPU) ||
		            (type == best_device.getInfo<CL_DEVICE_TYPE>() && compute_units > best_device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>())) {
		            best_device = dev;
		            best_platform = platform;
		            found = true;
		        }
	device_cnt++;
		    }
      platform_cnt++;
		}

		if (!found) {
		    throw std::runtime_error("No suitable OpenCL devices found.");
		}

		std::cout << "Selected device: " << best_device.getInfo<CL_DEVICE_NAME>()
		          << " on platform: " << best_platform.getInfo<CL_PLATFORM_NAME>() << "\n";

		// Use best_device and best_platform to create context and queue:
    active_device = best_device;
    active_platform = best_platform;
		context = cl::Context(best_device);
		queue = cl::CommandQueue(context, best_device);
	}

  void CL_context::activate(int platform_id, int device_id){
    this->active_platform = this->platforms[platform_id];
    this->active_platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
    this->active_device = this->devices[device_id];
    this->context = cl::Context(this->active_device);
    this->queue = cl::CommandQueue(this->context, this->active_device);
    std::cout << "Switched device: " << this->active_device.getInfo<CL_DEVICE_NAME>()
	      << " on platform: " << this->active_platform.getInfo<CL_PLATFORM_NAME>() << "\n";    
  }
  
	CL_context::~CL_context() {}

	CL_context& CL_context::instance() {
	    static CL_context instance;
	    return instance;
	}

	void CL_context::set_kernels_path(const std::string& path) {
	    std::lock_guard<std::mutex> lock(mutex);
	    kernels_path = path;
	}

	std::string CL_context::get_kernels_path() const {
	    std::lock_guard<std::mutex> lock(mutex);
	    return kernels_path;
	}

    const cl::Context& CL_context::get_context() const {
    	return context;
    }

	const cl::CommandQueue& CL_context::get_queue() const {
		return queue;
	}

	const cl::Device& CL_context::get_device() const {
		return best_device;
	}


	cl::Program CL_context::get_program(const std::string& filename) const {
		std::ifstream file(kernels_path + "/" + filename);
	    if (!file.is_open()) {
	        throw std::runtime_error("Failed to open kernel file: " + filename);
	    }
	    std::stringstream ss;
	    ss << file.rdbuf();
	    std::string kernel_str = ss.str();

	    std::cout << kernel_str << std::endl;

		cl::Program::Sources sources;
		sources.push_back({kernel_str.c_str(), kernel_str.length()});
        cl::Program program(context, sources);

    //    cl_int err = program.build("-cl-fast-relaxed-math");
    cl_int err = program.build("");
        if (err != CL_SUCCESS) {
            std::string buildlog = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(best_device);
            throw std::runtime_error("Error building: " + buildlog + "\n");
        }

        return program;
	}

}
