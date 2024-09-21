#ifndef CAMERA_H
#define CAMERA_H

#include <libattopng.h> // for pngs
#include <pthread.h> //multithreading 

#include "config.h"
#include "vector3.h"
#include "color.h"
#include "ray.h"
#include "hittable.h"
#include "random.h"
#include "material.h"

typedef struct
{
    int image_width; 
    int image_height; 
    int samples_per_pixel; 
    double pixel_samples_scale; 
    int max_depth; 
    double viewport_width; 
    double viewport_height; 
    Point3 center; 
    Point3 viewport_upper_left;
    Point3 pixel00_loc; 
    Vec3 pixel_delta_u; 
    Vec3 pixel_delta_v; 
    double defocus_angle; 
    double focus_dis; 
    Vec3 defocus_disk_u; 
    Vec3 defocus_disk_v; 
} Camera; 

typedef struct 
{
    libattopng_t* png; 
    Camera* camera; 
    Hittable** hittables;
    size_t hittables_size; 
    int start_x; 
    int start_y; 
    int end_x; 
    int end_y; 
} Render_thread_in; 


void Camera_init(Camera* camera); 

void Camera_render(Camera* camera, Hittable* hittables[], size_t hittables_size, int threads);

void *Camera_render_thread(void* rt_i);

#endif
