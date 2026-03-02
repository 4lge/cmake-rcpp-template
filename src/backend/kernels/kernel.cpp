#include "kernel.hpp" // note: unbalanced round brackets () are not allowed and string literals can't be arbitrarily long, so periodically interrupt with )+R(
string opencl_c_container() { return
    R( // ########################## begin of OpenCL C code ####################
       
#if CONFIG_USE_DOUBLE

#if defined(cl_khr_fp64)  // Khronos extension available?
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#define DOUBLE_SUPPORT_AVAILABLE
#elif defined(cl_amd_fp64)  // AMD extension available?
#pragma OPENCL EXTENSION cl_amd_fp64 : enable
#define DOUBLE_SUPPORT_AVAILABLE
#endif

#endif // CONFIG_USE_DOUBLE

#if defined(DOUBLE_SUPPORT_AVAILABLE)

// double
typedef double real_t;
typedef double2 real2_t;
typedef double3 real3_t;
typedef double4 real4_t;
typedef double8 real8_t;
typedef double16 real16_t;
#define PI 3.14159265358979323846

#else

// float
typedef float real_t;
typedef float2 real2_t;
typedef float3 real3_t;
typedef float4 real4_t;
typedef float8 real8_t;
typedef float16 real16_t;
#define PI 3.14159265359f

#endif

      )
    +R(
       // distance kernel

       kernel void distance_matrix(global real_t* output, global real_t* input,
				   const int N, const int DIM) {
	 size_t i = get_global_id(0);
	 size_t j = get_global_id(1);

	 if(i < N && j < i){
	   real_t tmpRes = 0;
	   for(int k = 0; k < DIM; ++k){
	     real_t diff = input[i + k*N] - input[j + k*N];
	     tmpRes += diff * diff;
	   }
	   tmpRes = sqrt(tmpRes);
	   output[j * N + i] = tmpRes;
	   output[i * N + j] = tmpRes;
	 }
       }

       // add kernel
       kernel void add_kernel(global real_t* A, global real_t* B, global real_t* C) { // equivalent to "for(uint n=0u; n<N; n++) {", but executed in parallel
	 const uint n = get_global_id(0);
	 C[n] = A[n]+B[n];
       }

       // multiply kernel
       kernel void mul_kernel(global real_t* A, global real_t* B, global real_t* C) { // equivalent to "for(uint n=0u; n<N; n++) {", but executed in parallel
	 const uint n = get_global_id(0);
	 C[n] = A[n]*B[n];
       }

       )
    +R(

       // add more utility functions and kernels here
       //
       // ...


       );} // ####################### end of OpenCL C code #####################

