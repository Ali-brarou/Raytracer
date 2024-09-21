#include "hittable.h"

#include <math.h> 
#include <stdio.h> //debugging
                   
void Hit_rec_face_norm(Hit_record* rec, Ray ray, Vec3 outward_norm)
{
    rec -> front_face = Vec3_dot(ray.dir, outward_norm) < 0; 

    rec -> normal = rec -> front_face ? outward_norm : Vec3_scale(outward_norm, -1);
}

bool hit_sphere(void* self, const Ray ray, Interval ray_t, Hit_record* rec)
{
    Sphere* sphere = (Sphere*) self;


    Vec3 oc = Vec3_sub(sphere -> center, ray.orig);
    

    double a = Vec3_len_sqr(ray.dir);
    double h = Vec3_dot(ray.dir, oc);
    double c = Vec3_len_sqr(oc) - (sphere -> radius * sphere -> radius); 

    double disc = h*h - a * c; 
    if (disc < 0.0)
        return false; 
    
    double sqrtd = sqrt(disc);

    double root = (h - sqrtd) / a; 
    if (!Interval_surrounds(ray_t, root))
    {
        root = (h + sqrtd) / a; 
        if (!Interval_surrounds(ray_t, root))
            return false; 
    }

    rec -> t = root;  
    rec -> p = Ray_at(ray, root);
    Vec3 outward_norm = Vec3_scale(Vec3_sub(rec -> p, sphere -> center),
            1.0 / sphere -> radius );
    Hit_rec_face_norm(rec, ray, outward_norm);
    rec -> material = sphere -> material; 

    return true; 

}

void Sphere_init(Sphere* sphere, Point3 center, double radius, Material* material)
{
    sphere -> hittable.hit = hit_sphere; 
    sphere -> radius = radius; 
    sphere -> center = center;  
    sphere -> material = material; 
}
