#include "material.h"

void Lambertian_init(Material* material, Color albedo)
{
    *material = (Material){
        .type = lambertian, 
        .albedo = albedo, 
        .fuzz = 0.0, 
        .refraction_index = 0.0, 
    };
}

void Metal_init(Material* material, Color albedo, double fuzz)
{
    *material = (Material){
        .type = metal, 
        .albedo = albedo, 
        .fuzz = fuzz, 
        .refraction_index = 0.0, 
    };
}


void Dielectric_init(Material* material, double refraction_index)
{
    *material = (Material){
        .type = dielectric, 
        .albedo = WHITE, 
        .fuzz = 0.0, 
        .refraction_index = refraction_index, 
    };
}

static inline double reflectance(double cosine, double refraction_index)
{
        double r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine),5);
}

bool Material_scatter(Material* material, Scattering_in in, Scattering_res* res)
{
    switch (material -> type)
    {
        case lambertian : 
        {
            Vec3 scatter_dir = Vec3_add(in.rec -> normal, Vec3_rand_unit());

            if (Vec3_near_zero(scatter_dir))
                scatter_dir = in.rec -> normal; 

            res -> scattared = (Ray){in.rec -> p, scatter_dir};
            res -> attenuation = material -> albedo; 
        }
        return true; 

        case metal : 
        {
            Vec3 reflected = Vec3_reflect(in.ray_i -> dir, in.rec -> normal);
            reflected = Vec3_add(Vec3_norm(reflected), 
                            Vec3_scale(Vec3_rand_unit(), material -> fuzz));
            res -> scattared = (Ray){in.rec -> p, reflected};
            res -> attenuation = material -> albedo;  
        return Vec3_dot(reflected, in.rec -> normal) > 0; 
            
        }

        case dielectric: 
        {
            res -> attenuation = WHITE; 
            double ri = in.rec -> front_face ? 1.0 / material -> refraction_index : 
                material -> refraction_index; 
            Vec3 unit_dir = Vec3_norm(in.ray_i -> dir);

            double cos_theta = fmin(Vec3_dot(Vec3_scale(unit_dir, -1), in.rec -> normal), 1.0);
            double sin_theta = sqrt(1.0 - pow(cos_theta, 2));

            bool cannot_refract = ri * sin_theta > 1.0;
        
            Vec3 direction; 
            if (cannot_refract || reflectance(cos_theta, ri) > random_double(0, 1.0))
                direction = Vec3_reflect(unit_dir, in.rec -> normal);
            else
                direction= Vec3_refract(unit_dir, in.rec -> normal, ri);

            res -> scattared = (Ray){in.rec -> p, direction};
        }
        return true; 
    }

    return false; 
}
