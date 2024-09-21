#ifndef MATERIAL_H
#define MATERIAL_H

#include "vector3.h"
#include "color.h"
#include "ray.h"
#include "hittable.h"


typedef enum 
{
    lambertian, 
    metal, 
    dielectric, 
} Material_type; 

typedef struct Material_s
{
    Material_type type; 
    Color albedo;      // For metals and lambertians only
    double fuzz;            // For metals only. Will be ignored for others materials
    double refraction_index;// For dielectrics only. Will be ignored for others materials
} Material; 

void Lambertian_init(Material* material, Color albedo);
void Metal_init(Material* material, Color albedo, double fuzz);
void Dielectric_init(Material* material, double refraction_index);

typedef struct 
{
    Ray scattared;    
    Color attenuation; 
} Scattering_res; 

typedef struct 
{
    Ray* ray_i; 
    Hit_record* rec; 
} Scattering_in; 

bool Material_scatter(Material* material, Scattering_in in, Scattering_res* res); 

#endif
