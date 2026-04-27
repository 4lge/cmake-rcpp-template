#include "backend/init.hpp"

//#include "CL_context.hpp"
#include "CLpp.hpp"

namespace backend {
  
  extern "C" void activateDeviceWithMostFlops() {
    CLpp::instance().activateDeviceWithMostFlops();
  }
  
  extern "C" void activateDeviceWithMostMemory() {
    CLpp::instance().activateDeviceWithMostMemory();
  }
  
  extern "C" void activateDeviceWithID(int id) {
    CLpp::instance().activateDeviceWithID(id);
  }
  
  extern "C" void setKernelsPath(const std::string& path) {
    CLpp::instance().setKernelsPath(path);
  }
  
  extern "C" void initCL() {
    CLpp::instance().activateDeviceWithMostFlops();
    CLpp::instance().info();
  }
}
