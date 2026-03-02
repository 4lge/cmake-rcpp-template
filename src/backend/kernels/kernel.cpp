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
	 return (real_t)mt_rand(&mt[0], &idx, mt) / 4294967295.0f;
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
    +R(

       // Exponential PDF
       real_t exponential_pdf(real_t x, real_t lambda) {
	 if (lambda <= 0 || x < 0) return 0.0f;
	 return lambda * exp(-lambda * x);
       }

       // Exponential CDF
       real_t exponential_cdf(real_t x, real_t lambda) {
	 if (lambda <=0 || x < 0) return 0.0f;
	 return 1.0f - exp(-lambda * x);
       }

       )
    +R(
       // Chi-Squared PDF
       real_t chi_squared_pdf(real_t x, real_t k) {
	 if (k <= 0 || x < 0) return 0.0f;
	 real_t log_pdf = (k/2.0f - 1.0f) * log(x) - x/2.0f - lgamma(k/2.0f) - (k/2.0f) * log(2.0f);
	 return exp(log_pdf);
       }
       
       // Chi-Squared CDF
       real_t chi_squared_cdf(real_t x, real_t k) {
	 if (k <= 0 || x < 0) return 0.0f;
    return 1.0f - exp(-x/2.0f) * pow(x/2.0f, k/2.0f) / (tgamma(k/2.0f) * pow(2.0f, k/2.0f));
       }
       
       )
    +R(
       // t-Distribution PDF
       real_t student_t_pdf(real_t x, real_t nu) {
	 if (nu <= 0) return 0.0f;
	 real_t log_pdf = lgamma((nu + 1.0f) / 2.0f) - lgamma(nu / 2.0f) - 0.5f * log(nu * M_PI);
	 log_pdf -= ((nu + 1.0f) / 2.0f) * log(1.0f + x * x / nu);
	 return exp(log_pdf);
       }

       // t-Distribution CDF (approximation via beta function)
       real_t student_t_cdf(real_t x, real_t nu) {
	 if (nu <= 0) return 0.0f;
	 real_t t = x / sqrt(nu + x * x);
	 real_t beta = 0.5f * (1.0f + t) * pow(1.0f - t * t, nu / 2.0f);
	 return (x < 0) ? beta : 1.0f - beta;
       }
       
       )
    +R(
       
       // Gamma PDF
       real_t gamma_pdf(real_t x, real_t shape_k, real_t scale_theta) {
	 if (shape_k <= 0 || scale_theta <= 0 || x < 0) return 0.0f;
	 real_t log_pdf = (shape_k - 1.0f) * log(x) - x / scale_theta - lgamma(shape_k) - shape_k * log(scale_theta);
	 return exp(log_pdf) / scale_theta;
       }
       
       // Gamma CDF
       real_t gamma_cdf(real_t x, real_t shape_k, real_t scale_theta) {
	 if (shape_k <= 0 || scale_theta <= 0 || x < 0) return 0.0f;
	 real_t sum = 0.0f;
	 for (int n = 0; n < 100; ++n) { // Approximate via series
	   sum += pow(x / scale_theta, shape_k + n) / tgamma(shape_k + n + 1);
	 }
	 return 1.0f - exp(-x / scale_theta) * sum;
       }

       
       )
    +R(
       // Beta PDF
       real_t beta_pdf(real_t x, real_t alpha, real_t beta_param) {
	 if (alpha <= 0 || beta_param <= 0 || x < 0 || x > 1) return 0.0f;
	 real_t log_pdf = (alpha - 1.0f) * log(x) + (beta_param - 1.0f) * log(1.0f - x) - lgamma(alpha + beta_param) + lgamma(alpha) + lgamma(beta_param);
	 return exp(log_pdf);
       }
       
       // Beta CDF (approximation via beta function)
       real_t beta_cdf(real_t x, real_t alpha, real_t beta_param) {
	 if (alpha <= 0 || beta_param <= 0 || x < 0 || x > 1) return 0.0f;
	 real_t sum = 0.0f;
	 for (int k = 0; k < 100; ++k) { // Approximate using series expansion
	   sum += pow(x, alpha + k) / (alpha + k) * pow(1.0f - x, beta_param + k) / (beta_param + k);
	 }
	 return sum;
       }

       )
    +R(
       // Weibull PDF
       real_t weibull_pdf(real_t x, real_t shape_k, real_t scale_lambda) {
	 if (shape_k <= 0 || scale_lambda <= 0 || x < 0) return 0.0f;
	 return (shape_k / scale_lambda) * pow(x / scale_lambda, shape_k - 1.0f) * exp(-pow(x / scale_lambda, shape_k));
       }
       
       // Weibull CDF
       real_t weibull_cdf(real_t x, real_t shape_k, real_t scale_lambda) {
	 if (shape_k <= 0 || scale_lambda <= 0 || x < 0) return 0.0f;
	 return 1.0f - exp(-pow(x / scale_lambda, shape_k));
       }

       )
    +R(

       // hypergeometric
       real_t log_choose(int a, int b) {
	 return lgamma((real_t)(a + 1)) - lgamma((real_t)(b + 1)) - lgamma((real_t)(a - b + 1));
       }
       // Compute hypergeometric PMF
       real_t compute_hyper_pmf(int k, int N, int K, int n) {
	 return exp(log_choose(K, k) + log_choose(N - K, n - k) - log_choose(N, n));
       }
       )
    +R(

       // Binomial PDF
       real_t binomial_pdf(int k, int n, real_t p) {
	 if (k < 0 || k > n || p < 0 || p > 1) return 0.0f;
	 real_t log_pmf = log_choose(n, k) + k * log(p) + (n - k) * log(1.0f - p);
	 return exp(log_pmf);
       }
       
       // Binomial CDF
       real_t binomial_cdf(int k, int n, real_t p) {
	 if (k < 0 || k > n || p < 0 || p > 1) return 0.0f;
	 real_t sum = 0.0f;
	 for (int i = 0; i <= k; ++i) {
	   sum += binomial_pdf(i, n, p);
	 }
	 return sum;
       }

       )
    +R(

       // Poisson PDF
       real_t poisson_pdf(int k, real_t lambda) {
	 if (k < 0 || lambda <= 0) return 0.0f;
	 real_t log_pmf = -lambda + k * log(lambda) - lgamma(k + 1.0f);
	 return exp(log_pmf);
       }
       
       // Poisson CDF
       real_t poisson_cdf(int k, real_t lambda) {
	 if (k < 0 || lambda <= 0) return 0.0f;
	 real_t sum = 0.0f;
	 for (int i = 0; i <= k; ++i) {
	   sum += poisson_pdf(i, lambda);
	 }
	 return sum;
       }
       )
    +R(
       // Geometric PDF
       real_t geometric_pdf(int k, real_t p) {
	 if (k < 0 || p <= 0 || p > 1) return 0.0f;
	 return p * pow(1.0f - p, k);
       }
       
       // Geometric CDF
       real_t geometric_cdf(int k, real_t p) {
	 if (k < 0 || p <= 0 || p > 1) return 0.0f;
	 return 1.0f - pow(1.0f - p, k + 1);
       }
       
       )
    +R(

       // Negative Binomial PDF
       real_t neg_binomial_pdf(int k, int r, real_t p) {
	 if (k < 0 || r <= 0 || p <= 0 || p > 1) return 0.0f;
	 real_t log_pmf = log_choose(k + r - 1, k) + r * log(p) + k * log(1.0f - p);
	 return exp(log_pmf);
       }
       
       // Negative Binomial CDF
       real_t neg_binomial_cdf(int k, int r, real_t p) {
	 if (k < 0 || r <= 0 || p <= 0 || p > 1) return 0.0f;
	 real_t sum = 0.0f;
	 for (int i = 0; i <= k; ++i) {
	   sum += neg_binomial_pdf(i, r, p);
	 }
	 return sum;
       }
       
       )
    // RNG functions
    +R(
       // uniform distribution
       real_t unif_rand(__private real_t lower, __private real_t upper,
			__private int* idx, __private uint* mt) {
	 return lower + (upper-lower)*mt_rand_01(&mt[0], &idx, mt);
       }

       // exponential distribution
       real_t exp_rand(__private real_t lambda,
		       __private int* idx, __private uint* mt) {
	 return lambda * (-log(mt_rand_01(&mt[0], &idx, mt)));
       }

       // normal distribution
       real_t norm_rand(__private real_t mu, __private real_t sigma,
			__private int* idx, __private uint* mt) {
	 return mu + sigma * sqrt(-2 * log(mt_rand_01(&mt[0], &idx, mt))) *
	   cos(2 * 3.14159265359f * mt_rand_01(&mt[0], &idx, mt));
	 // note we are wasting z1 = mu + sigma * sqrt(-2 * ln(u1)) * sin(2π * u2) here
       }

       // t distribution
       real_t t_rand(__private int nu, 
			__private int* idx, __private uint* mt) {
	 real_t V = nu * (-log(mt_rand_01(&mt[0], &idx, mt))); // Chi-squared with ν degrees of freedom
	 return sqrt(-2 * log(mt_rand_01(&mt[0], &idx, mt))) *
	   cos(2 * 3.14159265359f * mt_rand_01(&mt[0], &idx, mt)) * sqrt(nu / V);
       }

       // chisquared distribution
       real_t chisq_rand(__private int nu, 
			 __private int* idx, __private uint* mt) {
	 return nu * (-log(mt_rand_01(&mt[0], &idx, mt)));
       }
       

       // gamma distribution (shape theta, rate 1/k)
       real_t gamma_rand(__private real_t theta, __private real_t k,
			 __private int* idx, __private uint* mt) {
	 return theta * (-log(mt_rand_01(&mt[0], &idx, mt))) * k;
       }

       // beta
       real_t beta_rand(__private real_t alpha, __private real_t beta,
			__private int* idx, __private uint* mt) {
	 real_t gamma1 = alpha * (-log(mt_rand_01(&mt[0], &idx, mt)));
	 real_t gamma2 = beta * (-log(mt_rand_01(&mt[0], &idx, mt)));
	 return gamma1 / (gamma1 + gamma2);
       }

       // weibull
       real_t weibull_rand(__private real_t lambda, __private real_t k,
			 __private int* idx, __private uint* mt) {
	 return lambda * pow(-log(mt_rand_01(&mt[0], &idx, mt)), 1.0f / k); 
       }

       
       // binomial distribution (shape theta, rate 1/k)
       uint binom_rand(__private int size, __private real_t prob,
		       __private int* idx, __private uint* mt) {
	 uint count = 0;
	 for (int i = 0; i < size; ++i) {
	   count += (mt_rand_01(&mt[0], &idx, mt) < prob); // Requires multiple uniforms
	 }
	 return count;
       }

       // geometric
       uint geom_rand(__private real_t prob,
		      __private int* idx, __private uint* mt){
	 return floor(log(mt_rand_01(&mt[0], &idx, mt)) / log(1 - prob)) + 1;
       }

       // negative binomial
       uint nbinom_rand(__private int size,__private real_t prob, 
		      __private int* idx, __private uint* mt){
	 uint sum = 0;
	 for (int i = 0; i < size; ++i) {
	   sum += floor(log(mt_rand_01(&mt[0], &idx, mt)) / log(1 - prob)) + 1; // Requires multiple uniforms
	 }
	 return sum;
       }

       // poisson
       uint pois_rand(__private real_t lambda,
		      __private int* idx, __private uint* mt){
	 uint k = 0;
	 real_t sum_exponentials = 0.0;
	 while (sum_exponentials <= 1) { // Sum exponentials until exceed 1
	   sum_exponentials += -log(mt_rand_01(&mt[0], &idx, mt)) * lambda;
	   k++;
	 }
	 return k;
       }

       // hypergeometric
       uint hyperg_rand(__private int white, __private int black, __private int size,
			__private int* idx, __private uint* mt){
	 int N = white + black, K = white, n = size;
	 // Ratio-of-Uniforms parameters
	 real_t h = 1.0f; // Adjust based on PMF scaling

	 // Generate candidate k using u2 (assuming max_k = min(n, K))
	 int max_k = min(n, K);
	 int min_k = max(0, n - (N - K));
	 
	 // Compute PMF bounds and ratio-of-uniforms condition
	 real_t max_pmf = 0.0f;
	 for (int k = min_k; k <= max_k; ++k) {
	   // Compute log combinations to avoid overflow
	   real_t pmf = compute_hyper_pmf(k, N, K, n);
	   max_pmf = max(max_pmf, pmf);
	 }

	 while(true){
	   real_t u1 = mt_rand_01(&mt[0], &idx, mt);
	   real_t u2 = mt_rand_01(&mt[0], &idx, mt);
	   uint k_candidate = min_k + (int)(u2 * (max_k - min_k + 1));
	   real_t pmf = compute_hyper_pmf(k_candidate, N, K, n);
	   // Ratio check
	   if (u1 <= pmf * h / max_pmf) {
	     if(k_candidate<n)
	       return ++k_candidate; // Accepted k value
	   } 
	 }
       }
       )
    // RNG kernels    
    +R(
       kernel void norm_rng(global real_t* output, global int* seed, global real_t* mu, global real_t* sigma) {
	 
         // Initialize Mersenne Twister state
         uint mt[624];
         int idx = 624;
         mt[0] = *seed + get_global_id(0);
         for (int i = 1; i < 624; ++i) {
           mt[i] = (181243 * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i);
         }

         real_t r_norm = norm_rand(*mu, *sigma, &idx, mt);
         
         output[get_global_id(0)] = r_norm;
       }
       )
    // t kernel
    +R(
       kernel void t_rng(global real_t* output, global int* seed, global int* nu) {
	 
	 // Initialize Mersenne Twister state
	 uint mt[624];
	 int idx = 624;
	 mt[0] = *seed + get_global_id(0);
	 for (int i = 1; i < 624; ++i) {
	   mt[i] = (181243 * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i);
	 }

	 real_t r_t = t_rand(*nu, &idx, mt);

	 output[get_global_id(0)] = r_t;
       }

       )
    // chisq kernel
    +R(
       kernel void chisq_rng(global real_t* output, global int* seed, global int* nu) {
	 
	 // Initialize Mersenne Twister state
	 uint mt[624];
	 int idx = 624;
	 mt[0] = *seed + get_global_id(0);
	 for (int i = 1; i < 624; ++i) {
	   mt[i] = (181243 * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i);
	 }

	 real_t r_chisq = chisq_rand(*nu, &idx, mt);

	 output[get_global_id(0)] = r_chisq;
       }


       )
    // gamma kernel
    +R(
       kernel void gamma_rng(global real_t* output, global int* seed, global real_t* k, global real_t* theta) {
	 
	 // Initialize Mersenne Twister state
	 uint mt[624];
	 int idx = 624;
	 mt[0] = *seed + get_global_id(0);
	 for (int i = 1; i < 624; ++i) {
	   mt[i] = (181243 * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i);
	 }

	 real_t r_gamma = gamma_rand(*theta, *k, &idx, mt);

	 output[get_global_id(0)] = r_gamma;
       }

       )
    // beta kernel
    +R(
       kernel void beta_rng(global real_t* output, global int* seed, global real_t* alpha, global real_t* beta) {
	 
	 // Initialize Mersenne Twister state
	 uint mt[624];
	 int idx = 624;
	 mt[0] = *seed + get_global_id(0);
	 for (int i = 1; i < 624; ++i) {
	   mt[i] = (181243 * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i);
	 }

	 real_t r_beta = beta_rand(*alpha, *beta, &idx, mt);

	 output[get_global_id(0)] = r_beta;
       }


       )
    // exponential kernel
    +R(
       kernel void exp_rng(global real_t* output, global int* seed, global real_t* lambda) {
	 
	 // Initialize Mersenne Twister state
	 uint mt[624];
	 int idx = 624;
	 mt[0] = *seed + get_global_id(0);
	 for (int i = 1; i < 624; ++i) {
	   mt[i] = (181243 * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i);
	 }

	 real_t r_exp = exp_rand(*lambda, &idx, mt);

	 output[get_global_id(0)] = r_exp;
       }
       )              
    // weibull kernel
    +R(
       kernel void weibull_rng(global real_t* output, global int* seed, global real_t* lambda, global real_t* k) {
	 
	 // Initialize Mersenne Twister state
	 uint mt[624];
	 int idx = 624;
	 mt[0] = *seed + get_global_id(0);
	 for (int i = 1; i < 624; ++i) {
	   mt[i] = (181243 * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i);
	 }

	 real_t r_weibull = weibull_rand(*lambda, *k, &idx, mt);

	 output[get_global_id(0)] = r_weibull;
       }
              
       )
    +R(
       // uniform kernel
       kernel void unif_rng(global real_t* output, global int* seed,
			    global real_t* lower, global real_t* upper) {
	 
	 // Initialize Mersenne Twister state
	 uint mt[624];
	 int idx = 624;
	 mt[0] = *seed + get_global_id(0);
	 for (int i = 1; i < 624; ++i) {
	   mt[i] = (181243 * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i);
	 }

	 real_t r_unif = unif_rand(*lower, *upper, &idx, mt);

	 output[get_global_id(0)] = r_unif;
       }
       )
    +R(
       kernel void binom_rng(global int* output, global int* seed,
			     global int* size, global real_t* prob) {
	 
	 // Initialize Mersenne Twister state
	 uint mt[624];
	 int idx = 624;
	 mt[0] = *seed + get_global_id(0);
	 for (int i = 1; i < 624; ++i) {
	   mt[i] = (181243 * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i);
	 }

	 uint r_binom = binom_rand(*size, *prob, &idx, mt);

	 output[get_global_id(0)] = r_binom;
       }
       )
    +R(
       kernel void geom_rng(global int* output, global int* seed,
			    global real_t* prob) {
	 
	 // Initialize Mersenne Twister state
	 uint mt[624];
	 int idx = 624;
	 mt[0] = *seed + get_global_id(0);
	 for (int i = 1; i < 624; ++i) {
	   mt[i] = (181243 * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i);
	 }

	 uint r_geom = geom_rand(*prob, &idx, mt);

	 output[get_global_id(0)] = r_geom;
       }
       )
    +R(
       kernel void nbinom_rng(global int* output, global int* seed,
			      global int* size, global real_t* prob) {
	 
	 // Initialize Mersenne Twister state
	 uint mt[624];
	 int idx = 624;
	 mt[0] = *seed + get_global_id(0);
	 for (int i = 1; i < 624; ++i) {
	   mt[i] = (181243 * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i);
	 }
	 

	 uint r_nbinom = nbinom_rand(*size, *prob, &idx, mt);

	 output[get_global_id(0)] = r_nbinom;
       }
       )
    +R(
       kernel void pois_rng(global int* output, global int* seed,
			    global real_t* lambda) {
	 
	 // Initialize Mersenne Twister state
	 uint mt[624];
	 int idx = 624;
	 mt[0] = *seed + get_global_id(0);
	 for (int i = 1; i < 624; ++i) {
	   mt[i] = (181243 * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i);
	 }
	 
	 uint r_pois = pois_rand(*lambda, &idx, mt);

	 output[get_global_id(0)] = r_pois;
       }
       )
    +R(
       
       kernel void hyperg_rng(global int* output, global int* seed,
			      global int* size, global int* white, global int* black ) {
	 	 
	 // Initialize Mersenne Twister state
	 uint mt[624];
	 int idx = 624;
	 mt[0] = *seed + get_global_id(0);
	 for (int i = 1; i < 624; ++i) {
	   mt[i] = (181243 * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i);
	 }
	 
	 uint r_hyperg =hyperg_rand(*size, *white, *black, &idx, mt);

	 output[get_global_id(0)] = r_hyperg;
       }

       
       );} // ####################### end of OpenCL C code #####################
