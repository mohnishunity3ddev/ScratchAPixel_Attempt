#include <iostream>
#include <jmath.h>

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
    Vec3f crossResult = yAxis.cross(xAxis);

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

    Matrix44f invEx( 1,  1,  3,  0, 
                     1,  3, -3,  0, 
                    -2, -4, -4,  0, 
                     0,  0,  0,  1);
    invEx.invert();
    std::cout << invEx << std::endl;

    Matrix44f invEx2( 1,  1,  1, -1, 
                      1,  1, -1,  1, 
                      1, -1,  1,  1, 
                     -1,  1,  1,  1);
    Matrix44f inverse = invEx2.inverse();
    std::cout << "Matrix Example: " << invEx2 << std::endl;
    std::cout << "Inverse: " << inverse << std::endl;
    

    float determinant = invEx2.determinant();
    std::cout << "determinant is " << determinant << std::endl;

    Matrix44f invEx3( 4,  3,  2,  2, 
                      0,  1, -3,  3, 
                      0, -1,  3,  3, 
                      0,  3,  1,  1);
    determinant = invEx3.determinant();
    std::cout << "determinant is " << determinant << std::endl;
    
    Matrix44f invEx4( 1,  2,  3,  4, 
                      2,  5,  7,  3, 
                      4, 10, 14,  6, 
                      3,  4,  2,  7);
    determinant = invEx4.determinant();
    std::cout << "determinant is " << determinant << std::endl;
 
    return 0;                                                                 
}
