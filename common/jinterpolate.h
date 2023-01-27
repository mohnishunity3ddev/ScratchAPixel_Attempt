#if !defined JINTERPOLATE_H
#define JINTERPOLATE_H
#include <jmath.h>

struct cell_colors
{
    Vec3ui C00;
    Vec3ui C10;
    Vec3ui C01;
    Vec3ui C11;
};

Vec3f
bilinear(const float &tx, const float &ty, 
         const Vec3f &c00, const Vec3f &c10,
         const Vec3f &c01, const Vec3f &c11)
{
#if 1
    Vec3f a = c00 * (1.0f - tx) + c10 * tx;
    Vec3f b = c01 * (1.0f - tx) + c11 * tx;
    return a * (1.0f - ty) + b * (ty);
#else
#endif
}

Vec3f
bilinear(const float &tx, const float &ty, 
         const cell_colors &CellColors)
{
    Vec3f c00 {(float)CellColors.C00.x, (float)CellColors.C00.y, (float)CellColors.C00.z};
    Vec3f c10 {(float)CellColors.C10.x, (float)CellColors.C10.y, (float)CellColors.C10.z};
    Vec3f c01 {(float)CellColors.C01.x, (float)CellColors.C01.y, (float)CellColors.C01.z};
    Vec3f c11 {(float)CellColors.C11.x, (float)CellColors.C11.y, (float)CellColors.C11.z};
#if 1
    Vec3f a = c00 * (1.0f - tx) + c10 * tx;
    Vec3f b = c01 * (1.0f - tx) + c11 * tx;
    return a * (1.0f - ty) + b * (ty);
#else
#endif
}



#endif