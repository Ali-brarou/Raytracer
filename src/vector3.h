#ifndef VECTOR3_H
#define VECTOR3_H

#include <math.h> 
#include <stdbool.h>    //bool 
#include <sys/param.h>  //min
                   
#include "random.h" //random_double

typedef struct
{
    double x;
    double y;
    double z;
} Vec3; 

typedef Vec3 Point3; 

inline Vec3 Vec3_zero(void)
{
    return (Vec3){0, 0, 0};
}


inline Vec3 Vec3_add(const Vec3 v1, const Vec3 v2)
{
    return (Vec3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

inline Vec3 Vec3_sub(const Vec3 v1, const Vec3 v2)
{
    return (Vec3){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

inline Vec3 Vec3_mul(const Vec3 v1, const Vec3 v2)
{
    return (Vec3){v1.x * v2.x, v1.y * v2.y, v1.z * v2.z};
}

inline Vec3 Vec3_scale(const Vec3 v, double scale)
{
    return (Vec3){v.x * scale, v.y * scale, v.z * scale};
}

inline Vec3 Vec3_sqrt(Vec3 v)
{
    return (Vec3){sqrt(v.x), sqrt(v.y), sqrt(v.z)};
}

inline double Vec3_len_sqr(const Vec3 v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z; 
}

inline double Vec3_len(const Vec3 v)
{
    return sqrt(Vec3_len_sqr(v));
}

inline Vec3 Vec3_norm(const Vec3 v)
{
    return Vec3_scale(v, 1.0 / Vec3_len(v));
}

inline Vec3 Vec3_cross(const Vec3 v1, const Vec3 v2)
{
    return (Vec3){ v1.y * v2.z - v1.z * v2.y, 
                v1.z * v2.x - v1.x * v2.z, 
                v1.x * v2.y - v1.y * v2.x
    };
}

inline double Vec3_dot(const Vec3 v1, const Vec3 v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; 
}

inline Vec3 Vec3_rand(double min, double max)
{
    return (Vec3){random_double(min, max), 
                random_double(min, max), 
                random_double(min, max)};
}

inline Vec3 Vec3_rand_unit_in_sphere(void)
{
    while (1)
    {
        Vec3 p = Vec3_rand(-1.0, 1.0);
        if (Vec3_len_sqr(p) < 1)
        {
            return p; 
        }
    }
}

inline Vec3 Vec3_rand_unit(void)
{
    return Vec3_norm(Vec3_rand_unit_in_sphere());
}

inline Vec3 Vec3_rand_hemisphere(Vec3 normal)
{
    Vec3 on_unit_sphere = Vec3_rand_unit();
    return (Vec3_dot(on_unit_sphere, normal) > 0) ? on_unit_sphere : 
        Vec3_scale(on_unit_sphere, -1); 
    
}

inline Vec3 Vec3_rand_in_disk(void)
{
    while (1)
    {
        Vec3 p = (Vec3){random_double(-1, 1), random_double(-1, 1), 0};
        if (Vec3_len_sqr(p) < 1)
            return p; 
    }
}

inline Vec3 Vec3_reflect(Vec3 v, Vec3 normal)
{
    return Vec3_sub(v, Vec3_scale(normal, 2 * Vec3_dot(v, normal)));
}

inline bool Vec3_near_zero(Vec3 v)
{
    double s = 1e-8;

    return (fabs(v.x) < s) && (fabs(v.y) < s) && (fabs(v.z) < s);
}

inline Vec3 Vec3_refract(Vec3 v, Vec3 n, double etai_over_etat)
{
    double cos_theta = fmin(Vec3_dot(Vec3_scale(v, -1), n), 1.0);
    Vec3 r_out_perp =  Vec3_scale(Vec3_add(v, Vec3_scale(n, cos_theta)), etai_over_etat); 
    Vec3 r_out_parallel = Vec3_scale(n ,-sqrt(fabs(1.0 - Vec3_len_sqr(r_out_perp)))); 
    return Vec3_add(r_out_perp, r_out_parallel);
}

#endif
