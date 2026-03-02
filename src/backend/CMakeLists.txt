CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(BACKEND CXX)

# Get the Rcpp minGW/gcc compilers in order to achieve highest degree of uniformity while users only have to install Rtools.
include(cmake/find_toolchain.cmake)

# Pull the dependencies from Github
include(ExternalProject)
set(EXTERNAL_LIB_LOCATION ${CMAKE_SOURCE_DIR}/lib)
ExternalProject_Add(OpenCL_Headers
    GIT_REPOSITORY https://github.com/KhronosGroup/OpenCL-Headers.git
    GIT_TAG b79b358ff6b9998d3fc65a5f55083db3d651d55d
    SOURCE_DIR ${EXTERNAL_LIB_LOCATION}/OpenCL-Headers

    CONFIGURE_COMMAND ""  # Don't run cmake
    BUILD_COMMAND ""      # Don't build
    INSTALL_COMMAND ""    # Don't install
)
ExternalProject_Add(OpenCL_CLHPP
    GIT_REPOSITORY https://github.com/KhronosGroup/OpenCL-CLHPP.git
    GIT_TAG 2a608428f725cad7903ef55e1ce5b995895838f5
    SOURCE_DIR ${EXTERNAL_LIB_LOCATION}/OpenCL-CLHPP

    CONFIGURE_COMMAND ""  # Don't run cmake
    BUILD_COMMAND ""      # Don't build
    INSTALL_COMMAND ""    # Don't install
)
ExternalProject_Add(OpenCL_Wrapper
    GIT_REPOSITORY https://github.com/ProjectPhysX/OpenCL-Wrapper.git
    GIT_TAG ff34d194c0c82b71b458d77c7c339fa5958af038
    SOURCE_DIR ${EXTERNAL_LIB_LOCATION}/OpenCL-Wrapper

    CONFIGURE_COMMAND ""  # Don't run cmake
    BUILD_COMMAND ""      # Don't build
    INSTALL_COMMAND ""    # Don't install
)

ExternalProject_Add(CLErrorLookup
    GIT_REPOSITORY https://github.com/otto-link/CLErrorLookup/
    GIT_TAG 27adbb9217e2a9e31b08fe32acc21df9d2b53e84
    SOURCE_DIR ${EXTERNAL_LIB_LOCATION}/CLErrorLookup

    CONFIGURE_COMMAND ""  # Don't run cmake
    BUILD_COMMAND ""      # Don't build
    INSTALL_COMMAND ""    # Don't install
)



# Include the external library headers in the lib folder
include_directories(
  ${CMAKE_SOURCE_DIR}/lib/OpenCL-Headers
  ${CMAKE_SOURCE_DIR}/lib/OpenCL-CLHPP/include
  ${CMAKE_SOURCE_DIR}/lib/OpenCL-Wrapper/src
  ${CMAKE_SOURCE_DIR}/lib/CLErrorLookup
  ${OpenCL_INCLUDE_DIRS}
)
# find your binaries here
FIND_PACKAGE(OpenCL REQUIRED)


# Make a shared library
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE KERNELS "kernels/*.cpp")
ADD_LIBRARY(BACKEND SHARED ${SOURCES} ${KERNELS})

# The header includes go here
# Leave the two /include and /src/internal ones in here; the rest can be altered.
target_include_directories(BACKEND
  PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src/internal
		${OpenCL_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/lib/OpenCL-Headers
    ${CMAKE_SOURCE_DIR}/lib/OpenCL-CLHPP/include
    ${CMAKE_SOURCE_DIR}/lib/CLErrorLookup/CLErrorLookup/include
)

# Linkage against your binaries
target_link_libraries(BACKEND
  PRIVATE
    ${OpenCL_LIBRARIES}
    OpenCL::OpenCL
)

# some OS-specific library name conventions here; just leave as is as it would change the way Rcpp's Makefile needs to look.
if(WIN32)
  SET_TARGET_PROPERTIES(BACKEND PROPERTIES
    OUTPUT_NAME "BACKEND"
    PREFIX ""
  )
endif()
