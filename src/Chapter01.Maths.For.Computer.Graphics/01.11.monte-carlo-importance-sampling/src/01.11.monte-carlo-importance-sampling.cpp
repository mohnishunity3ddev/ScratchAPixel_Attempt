#include <cmath>
#include <cstdio>
#include <cstdlib>

// NOTE:
// Here, we are trying to find the Integral Limit(0->Pi/2) sin(x)dx
// We use two functions one is a uniform distribution - which is a straight line
// parallel to the x-axis ie 2/pi The other function is 8x/pi^2. This function
// closely resembles the function sinx so our estimate using this function in
// the importance sampling  algo should give out much less error/variance in the
// final result.
//
// The final solution of this question is 1 using our calculus formulas for sinx
// limit 0 to pi/2.

#define PI 3.14159f

inline float
Rand01()
{
    float Result = (float)rand() / (float)RAND_MAX;
    return Result;
}

int
main()
{
    const int nSamples = 32;
    
    for(int j = 0; j < 16; ++j)
    {
        float sumUniform    = 0.0f;
        float sumImportance = 0.0f;

        for (int i = 0; i < nSamples; ++i)
        {
            // Value between 0 and 1 uniformly distributed.
            float r = Rand01();
            
            // The domain of the integrand since the limit is from 0->pi/2 is pi/2 *
            // (0, 1) value.
            sumUniform += sinf(r * PI * .5f);
            
            // This is the result of inverse cdf sampling where invert the CDF of
            // the function that we have to transform the 0,1 value to match the pdf
            // of the function provided. the goal is to match the functionin the
            // denominator as closely to the given function as possible for this to
            // work.
            // The cdf of the given function 8x/pi^2:  4x^2/pi^2
            // The inverse of this cdf is: sqrt(x)*pi/2 
            float xi = sqrtf(r) * PI * 0.5f;
            // Applying the formula for importance sampling which is: summation[f(x) / pdf(x)]
            sumImportance += sinf(xi) / ((8.0f*xi) / (PI*PI));
        }

        sumUniform *= (PI * 0.5f) / nSamples;
        sumImportance *= 1.0f / nSamples;

        printf("Uniform Sampling: %f, Importance Sampling: %f, ErrorUniform: "
               "%f, ErrorImportance: %f.\n",
               sumUniform, sumImportance, std::abs(sumUniform - 1.0f),
               std::abs(sumImportance - 1.0f));
    }
    
    
    return 0;
}
