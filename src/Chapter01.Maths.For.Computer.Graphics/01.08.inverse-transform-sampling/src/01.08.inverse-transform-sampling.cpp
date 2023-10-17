#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <algorithm>

#define M_PI 3.141592653589793

inline
float Rand01()
{
    float Result = (float)rand() / (float)RAND_MAX;
    return Result;
}

// standard normal distribution function
float
pdf(const float &x)
{
    return 1 / sqrtf(2 * M_PI) * exp(-x * x * 0.5);
}

// NOTE: 
// cdf here is the cumulative distribution function so it adds the previous
// probability with the current probability. Its maximum value is 1. and it is a
// non-decreasing function.
// Here, we are already using a uniform distribution inside this function which
// returns a value between 0 and 1. But, we could also modify this to accept
// some uniform distribution function. The cdf here, is the one we want to
// simulate.
float
sample(float *cdf, const uint32_t &nbins, const float &minBound,
       const float &maxBound)
{
    // get a random number between 0 and 1.
    float r = Rand01();
    
    // get the maximum number which is just lower than r.
    // this retuns the address of this lower_bound value in the array.
    float *ptr = std::lower_bound(cdf, cdf + nbins + 1, r);
    
    // get the index into the array which has the lower bound.
    int off = std::max(0, (int)(ptr - cdf - 1));
    
    // get how much the r value we had earlier from the Uniform PDF
    // is between the lower bound and upper bound. it is a value between 0
    // and 1.
    float t = (r - cdf[off]) / (cdf[off + 1] - cdf[off]);
    
    // get how far the value is between the global minBound and global maxBound.
    //
    // NOTE: Here, we are getting the inverse of the cdf. We provide it with a
    // 0-1 value which is on its y-axis and get the x value. So this is how we
    // invert an arbitrary cdf.
    float x = (off + t) / (float)(nbins);
    
    float result = minBound + (maxBound - minBound) * x;
    
    return result;
}

int
main()
{
    srand(13); // seed random generator
    // create CDF
    int nbins      = 32;
    float minBound = -5, maxBound = 5;
    float cdf[nbins + 1], dx = (maxBound - minBound) / nbins, sum = 0;
    cdf[0] = 0.f;
    for (int n = 1; n < nbins; ++n)
    {
        float x     = minBound + (maxBound - minBound) * (n / (float)(nbins));
        float pdf_x = pdf(x) * dx;
        cdf[n]      = cdf[n - 1] + pdf_x;
        sum += pdf_x;
    }
    cdf[nbins] = 1;
    
    // our simulation
    int numSims = 100000;
    int numBins = 100;                        // to collect data on our sim
    int bins[numBins];                        // to collect data on our sim
    memset(bins, 0x0, sizeof(int) * numBins); // set all the bins to 0
    const float dist = 10;                    // 10 km
    for (int i = 0; i < numSims; ++i)
    {
        // random var between -5 and 5
        // NOTE: This basically returns the values which tell how late/early the
        // train was from its official arrival time.
        float diff = sample(cdf, nbins, minBound, maxBound); 
        int whichBin = (int)(numBins * (diff - minBound) / (maxBound - minBound));
        bins[whichBin]++;
        
/*         
        float time  = 30 + diff;
        float speed = 60 * dist / time;
*/    
    }
    
    sum = 0;
    for (int i = 0; i < numBins; ++i)
    {
        float r = bins[i] / (float)numSims;
        printf("%f %f\n", 5 * (2 * (i / (float)(numBins)) - 1), r);
        sum += r;
    }
    fprintf(stderr, "sum %f\n", sum);
    return 0;
}
