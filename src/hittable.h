#ifndef HITTABLE_H
#define HITTABLE_H

#include <stdbool.h> 

#include "vector3.h"
#include "ray.h"
#include "interval.h"

typedef struct Material_s Material; 

typedef struct Hit_record_s
{
    Point3 p; 
    Vec3 normal; 
    double t; 
    bool front_face;
    Material* material; 
} Hit_record; 

void Hit_rec_face_norm(Hit_record* rec, Ray ray, Vec3 outward_norm);

typedef struct 
{
    bool (*hit)(void*, const Ray, Interval, Hit_record*);
} Hittable; 

bool hit_sphere(void* self, const Ray ray, Interval ray_t, Hit_record* rec);

typedef struct  // yes polymorphism in c
{
    Hittable hittable; 
    //private variables
    Point3 center;
    double radius; 
    Material* material; 
} Sphere;

void Sphere_init(Sphere* sphere, Point3 center,double radius, Material* material);



#endif
