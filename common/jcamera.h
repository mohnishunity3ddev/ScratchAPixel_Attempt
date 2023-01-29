#if !defined(J_CAMERA_H)
#define J_CAMERA_H
#include "jmath.h"

class Camera
{
  public:
    Vec3f Position;
    Vec3f Forward;
    Camera()
    {
        Position = Vec3f{};
        Forward = Vec3f{0, 0, 1};
        Up = Vec3f{0, 1, 0};
    }

    Camera(const Vec3f &position, const Vec3f &forward)
    {
        Position = position;
        Forward = forward;
        
        Up = Vec3f{0, 1, 0};
        WorldUp = Vec3f{0, 1, 0};
        Right = Vec3f{1, 0, 0};
    }
    
    void LookAt(const Vec3f& from, const Vec3f &to, const Vec3f &worldUp, Matrix44f &m)
    {
        this->Position = from;
        Forward = (from - to).normalize();
        
        WorldUp = worldUp;
        WorldUp.normalize();
        
        Right = WorldUp.cross(Forward); // normalized
        Right.normalize();
    
        Up = Forward.cross(Right);
        Up.normalize();

        m[0][0] = Right.x; m[0][1] = Right.y; m[0][2] = Right.z; m[0][3] = 0.0f;
        m[1][0] = Up.x; m[1][1] = Up.y; m[1][2] = Up.z; m[1][3] = 0.0f;
        m[2][0] = Forward.x; m[2][1] = Forward.y; m[2][2] = Forward.z; m[2][3] = 0.0f;
        m[3][0] = from.x; m[3][1] = from.y; m[3][2] = from.z; m[3][3] = 1.0f;
    }
  
  private:
    Vec3f Up;
    Vec3f Right;
    Vec3f WorldUp;
};


#endif