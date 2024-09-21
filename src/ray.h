#ifndef RAY_H
#define RAY_H

#include "vector3.h"

typedef struct 
{
    Point3 orig; 
    Vec3 dir; 
} Ray; 

inline Point3 Ray_at(Ray ray, double t)
{
    return Vec3_add(ray.orig, Vec3_scale(ray.dir,t));
}


#endif 
