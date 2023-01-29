#include <iostream>

#include <jcamera.h>
#include <jmath.h>

int main() {
    Matrix44f m{};
    
    Vec3f up{0, 1, 0};
    Vec3f from{1, 1, 1};
    Vec3f to{0, 0, 0};

    Camera camera{};
    camera.LookAt(from, to, up, m);

    std::cout << m << std::endl;
    
    return 0;
}
