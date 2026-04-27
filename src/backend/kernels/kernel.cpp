#include "kernel.hpp" // note: unbalanced round brackets () are not allowed and string literals can't be arbitrarily long, so periodically interrupt with )+R(




  string opencl_c_container() {
    string OpenCL_C = R( // ########################## begin of OpenCL C code ####################
       

#if defined(cl_khr_fp64)  // Khronos extension available?
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#define DOUBLE_SUPPORT_AVAILABLE
#elif defined(cl_amd_fp64)  // AMD extension available?
#pragma OPENCL EXTENSION cl_amd_fp64 : enable
#define DOUBLE_SUPPORT_AVAILABLE
#endif

#if defined(DOUBLE_SUPPORT_AVAILABLE)

// double
typedef double real_t;
typedef double2 real2_t;
typedef double3 real3_t;
typedef double4 real4_t;
typedef double8 real8_t;
typedef double16 real16_t;
#define PI 3.14159265358979323846
#define REAL_ONE 1.0

#else

// float
typedef float real_t;
typedef float2 real2_t;
typedef float3 real3_t;
typedef float4 real4_t;
typedef float8 real8_t;
typedef float16 real16_t;
#define PI 3.14159265359f
#define REAL_ONE 1.0f

#endif

       )
    +R(
       
       // Uniform random number generator (Mersenne Twister)
       uint mt_rand(__private uint* state, __private int* idx, __private uint* mt) {
         if (*idx >= 624) {
           // Twist the state array
           for (int i = 0; i < 624; ++i) {
             uint temp = mt[i] ^ (mt[(i + 1) % 624] >> 1);
             temp ^= (temp >> 1) ^ ((temp & 1) ? 0x8EB8B000 : 0);
             mt[i] = mt[(i + 397) % 624] ^ temp;
           }
           *idx = 0;
         }
         uint x = mt[*idx];
         x ^= (x >> 11);
         x ^= (x << 7) & 0x9D2C5680;
         x ^= (x << 15) & 0xEFC60000;
         x ^= (x >> 18);
         (*idx)++;
         return x;
       }
       
       // Mersenne Twister scaled to [0,1]
       real_t mt_rand_01(__private uint* state, __private int* idx, __private uint* mt) {
         return (real_t)mt_rand(&mt[0], &idx[0], mt) / 4294967295.0f;
       }
       )
    // PDFs and CDFs    
    +R(

       // Normal Distribution (PDF)
       real_t normal_pdf(real_t x, real_t mu, real_t sigma) {
         if (sigma <= 0) return 0.0f;
         real_t z = (x - mu) / sigma;
         return (1.0f / (sigma * sqrt(2 * M_PI))) * exp(-0.5f * z * z);
       }
       
       // Normal CDF Approximation (using erf approximation)
       real_t normal_cdf(real_t x, real_t mu, real_t sigma) {
         if (sigma <= 0) return 0.0f;
         real_t z = (x - mu) / sigma;
         real_t t = 1.0f / (1.0f + 0.5f * fabs(z));
         real_t erf_approx = 1.0f - t * exp(-z * z - 1.26523f * t * (t - 1.0f + 0.045f * t));
         return 0.5f * (1.0f + sign(z) * erf_approx);
       }
       
       )
    // RNG functions
    +R(
       // uniform distribution
       real_t unif_rand(__private real_t lower, __private real_t upper,
                        __private int* idx, __private uint* mt) {
         return lower + (upper-lower)*mt_rand_01(&mt[0], &idx[0], mt);
       }

       // normal distribution
       real_t norm_rand(__private real_t mu, __private real_t sigma,
                        __private int* idx, __private uint* mt) {
         return mu + sigma * sqrt(-2 * log(mt_rand_01(&mt[0], &idx[0], mt))) *
           cos(2 * 3.14159265359f * mt_rand_01(&mt[0], &idx[0], mt));
         // note we are wasting z1 = mu + sigma * sqrt(-2 * ln(u1)) * sin(2π * u2) here
       }
    // RNG kernels    
    +R(
       // norm_rng moved to cl file
       )
    +R(
       // uniform kernel moved to cl file
       )
  } // ####################### end of OpenCL C code #####################
