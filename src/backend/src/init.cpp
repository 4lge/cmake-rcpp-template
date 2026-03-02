#include "backend/init.hpp"

//#include "CL_context.hpp"
#include "CLpp.hpp"

namespace backend {

  /*
	extern "C" void set_kernels_path(const std::string& path) {
	    CL_context::instance().set_kernels_path(path);
	    CL_context::instance().activate(0,0);
	}
  */
  	extern "C" void initCL() {
	  CLpp::instance().info();
	}
}