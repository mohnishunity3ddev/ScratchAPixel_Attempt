#if defined __linux__ || defined __APPLE__ 
// "Compiled for Linux
#else 
// Windows doesn't define these values by default, Linux does
#define M_PI 3.141592653589793 
#ifndef INFINITY
#define INFINITY 1e8 
#endif
#endif 