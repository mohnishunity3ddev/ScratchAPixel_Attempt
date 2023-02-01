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

int
main(int argc, char **argv)
{
    // Vec3f cartesianCoords(1, 2, 3);
    // SphericalCoordf sphereCoords{};
    // SphericalCoordf::cartesianToSpherical(cartesianCoords, &sphereCoords);

    // std::cout << "sphereCoords: " << sphereCoords << std::endl;

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
