#pragma once

#include <string>

#include "backend/export.hpp"

namespace backend {

	/**
	 * 	Attaches a kernel path to the static linkage OpenCL state singleton.
	 */
  extern "C" EXPORT void activateDeviceWithMostFlops();
  extern "C" EXPORT void activateDeviceWithMostMemory();
  extern "C" EXPORT void activateDeviceWithID(int id);
  extern "C" EXPORT void setKernelsPath(const std::string& path);
  extern "C" EXPORT void initCL();
}
