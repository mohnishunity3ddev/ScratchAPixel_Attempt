// #include <iostream>
#include <cstdio>
#include <cstdlib>
#include <jmath.h>

// anti-derivative
float
F(float x)
{
    return 2 * x * x;
}
// derivative
float
f(float x)
{
    return 4 * x;
}

union uFloat
{
    float f;
    unsigned int u;
};

void
PrintBinaryFloat(uFloat f)
{
    unsigned int u = f.u;
    printf("Floating point binary representation: ");
    for(uint32_t i = 0; i < 32; ++i)
    {
        if(i > 0 && i % 4 == 0) printf(" ");
        if((u >> (31-i)) & 1) printf("1");
        else printf("0");
    }
    printf(".\n");
    

    if(f.f > 0)
    {
        printf("Sign Bit(1): 0 (positive float).\n");
    }
    else
    {
        printf("Sign Bit(1): 1 (negative float).\n");
    }
    
    printf("Exponent(8): ");
    uint32_t exp = 0;
    
    for(int i = 0; i < 8; ++i)
    {
        int t = 30 - i;
        bool bitset = false;

        if(i > 0 && i % 4 == 0) printf(" ");
        
        if((u >> t) & 1) 
        {
            printf("1");
            bitset = true;
        }
        else
        {
            printf("0");
        } 
        
        if(bitset)
            exp = exp | (1 << (7-i)); 
    }
    printf(" (%u)\n", exp);
    
    printf("Mantissa(23): ");
    exp = 0;
    for(int i = 0; i < 23; ++i)
    {
        int t = 22 - i;
        bool bitset = false;
        if(i > 0 && i % 4 == 0) printf(" ");
        
        if((u >> t) & 1) 
        {
            printf("1");
            bitset = true;
        }
        else
        {
            printf("0");
        } 
        
        if(bitset)
            exp = exp | (1 << (22-i)); 
    }
    printf(" (%u)\n", exp);
}

int
main(int argc, char **argv)
{
    // Vec3f cartesianCoords(1, 2, 3);
    // SphericalCoordf sphereCoords{};
    // SphericalCoordf::cartesianToSpherical(cartesianCoords, &sphereCoords);
    // std::cout << "sphereCoords: " << sphereCoords << std::endl;
    
    
    uFloat myFloat = {};
    myFloat.f = -8.5f;
    
    PrintBinaryFloat(myFloat);
        
    float a = atof(argv[1]);
    float b = atof(argv[2]);
    int   N = atoi(argv[3]);
    N *= 100;
    float exact = F(b) - F(a);

    // NOTE: This sum used here is basically the area under the curve 4x taken
    // as a reimann sum.

    // compute approximation using Reimann Sum
    float dt = (b - a) / N, time = dt, sum = 0;
    for (int i = 1; i <= N; ++i)
    {
        float delta_d = f(time) * dt;
        sum += delta_d;
        time += dt;
    }
    
    printf("Result, exact %f, approximation %f, diff %f\n", exact, sum,
           sum - exact);

    return 0;
}
