#include <iostream>
#include <linear_algebra/jvec3.h>

int main() {
    Vec3f myFloat{ 1.0f, 2.0f, 3.0f };
    std::cout << "MyFloat => (" << myFloat.x << ", " << myFloat.y << ", " << myFloat.z << ")\n";
    return 0;
}
