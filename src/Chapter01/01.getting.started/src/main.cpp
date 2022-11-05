#include <iostream>
#include <linear_algebra/jmath.h>

int main() {
    Vec3f myFloat{ 1.0f, 2.0f, 3.0f };
    Vec3f f(1.0f);
    std::cout << "f = " << f << "\n";
    Vec3f rhs(1, 2, 3);
    std::cout << "rhs = " << rhs << "\n";
    Vec3f g = f + rhs;
    std::cout << "g(after add) => " << g << "\n";
    g = f - rhs;
    std::cout << "g(after subtraction) => " << g << "\n";
    g = f * 2.0f;
    std::cout << "g(after multiplying by 2) => " << g << "\n";

    std::cout << "dot of " << f << " and " << rhs << " is: " << f.dotProduct(rhs) << "\n";

    Vec3f xAxis{ 1, 0, 0 };
    Vec3f yAxis{ 0, 1, 0 };
    Vec3f crossResult = yAxis.crossProduct(xAxis);

    std::cout << yAxis << " cross " << xAxis << " is: " << crossResult << "\n";

    Vec3f allOnes(1.0f);
    std::cout << "Length of " << allOnes << " is: " << allOnes.length() << "\n";

    for(uint32_t i = 0; i < 3; ++i) {
        std::cout << "myFloat[" << i << "] = " << myFloat[i] << " before modification.\n";
        myFloat[i] = 0;
    }

    std::cout << "myFloat = " << myFloat << "\n";

    f.normalize();
    std::cout << "f after normalization: " << f << "\n";

    Vec3f defVec;
    std::cout << "Default vector: " << defVec << "\n";

    Vec3f v(0, 1, 2); 
    std::cerr << v << std::endl; 
    Matrix44f a, b, c; 
    c = a * b; 
 
    Matrix44f d(0.707107, 0, -0.707107, 0, -0.331295, 0.883452, -0.331295, 0, 0.624695, 0.468521, 0.624695, 0, 4.000574, 3.00043, 4.000574, 1); 
    std::cerr << d << std::endl; 
    d.invert(); 
    std::cerr << d << std::endl; 
 
    return 0; 

    
    return 0;
}
