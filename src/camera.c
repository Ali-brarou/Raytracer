#include "camera.h"

#include <stdlib.h> 
#include <math.h> 
#include <unistd.h> 

#define TO_RAD(x)\
    (x * M_PI / 180.0)


static inline bool hit_world(Ray ray, Hittable* hittables[], size_t hittables_size,Hit_record* rec)
{
    Interval interval = {0.001, INFINITY}; 
    bool did_hit_anything = false; 

    for (size_t i = 0; i < hittables_size; i++) // loop throw every hittable
    {
        Hit_record tmp_rec; 
        Hittable* hittable = hittables[i];

        bool did_hit = hittable -> hit(hittable, ray, interval, &tmp_rec); 

        if (did_hit)
        {
            did_hit_anything = true; 
            *rec = tmp_rec; 
            interval.max = tmp_rec.t; 
        }
    }

    return did_hit_anything; 
}


static inline Color Ray_color(Ray ray, Hittable* hittables[], size_t hittables_size,int depth) // The important stuff
{
    if (depth == 0)
        return (Color){0, 0, 0};

    Hit_record rec = {0}; 

    if (hit_world(ray, hittables, hittables_size,&rec))
    {
        Scattering_in in = {&ray, &rec};
        Scattering_res res = {0};
        if (Material_scatter(rec.material, in, &res))
        {
            return Vec3_mul(res.attenuation, Ray_color(res.scattared, hittables, hittables_size,depth - 1));
        }
        return (Color){0, 0, 0};
    }

    //draw the background 

    Vec3 unit_dir = Vec3_norm(ray.dir);

    double a = (unit_dir.y + 1.0) * 0.5; 

    Color start_color = {1.0, 1.0, 1.0};
    //Color start_color = BLACK; 
    Color end_color = {0.5, 0.7, 1.0};
    //Color end_color = WHITE;

    return Vec3_add(Vec3_scale(start_color, 1.0 - a), Vec3_scale(end_color, a));
}

void Camera_init(Camera* camera)
{

    camera -> image_width = IMAGE_WIDTH; 
    camera -> image_height = IMAGE_WIDTH / ASPECT_RATIO;
    camera -> focus_dis = FOCUS_DIS; 
    camera -> defocus_angle = DEFOCUS_ANGLE; 
    double theta = TO_RAD(VFOV);
    double h = tan(theta/2);
    camera -> viewport_height = 2.0 * h * camera -> focus_dis; 
    camera -> viewport_width = camera -> viewport_height * 
        ((double)(camera -> image_width) / camera -> image_height);
    camera -> center = LOOKFROM; 

    Vec3 u, v, w; 

    w = Vec3_norm(Vec3_sub(LOOKFROM, LOOKAT));
    u = Vec3_norm(Vec3_cross(VUP, w)); 
    v = Vec3_cross(w, u);

    Vec3 viewport_u = Vec3_scale(u, camera -> viewport_width);
    Vec3 viewport_v = Vec3_scale(v, -camera -> viewport_height);
    


    camera -> pixel_delta_u = Vec3_scale(viewport_u, 1.0 / camera -> image_width);
    camera -> pixel_delta_v = Vec3_scale(viewport_v, 1.0 / camera -> image_height);

    //init the viewport 
    camera -> viewport_upper_left = Vec3_sub(camera -> center, 
                                    Vec3_scale(w, camera -> focus_dis));
    camera -> viewport_upper_left = Vec3_sub(camera -> viewport_upper_left, 
                                    Vec3_scale(viewport_u, 0.5));
    camera -> viewport_upper_left = Vec3_sub(camera -> viewport_upper_left, 
                                    Vec3_scale(viewport_v, 0.5));

    camera -> pixel00_loc = Vec3_add(camera -> viewport_upper_left, 
            Vec3_scale(
                Vec3_add(camera -> pixel_delta_u, camera->pixel_delta_v), 0.5));


    //antinalising
    camera -> samples_per_pixel = SAMPLES_PER_PIXEL;
    camera -> pixel_samples_scale = 1.0 / SAMPLES_PER_PIXEL;

    //depth 
    camera -> max_depth = MAX_DEPTH; 

    //defocus
    double defocus_radius = camera -> focus_dis * tan(TO_RAD(camera -> defocus_angle / 2.0));

    camera -> defocus_disk_u = Vec3_scale(u, defocus_radius);
    camera -> defocus_disk_v = Vec3_scale(v, defocus_radius);
}

static inline Point3 defocus_disk_sample(Camera* camera)
{
    Vec3 p = Vec3_rand_in_disk();
    Vec3 res = Vec3_add(camera -> center, 
            Vec3_scale(camera -> defocus_disk_u, p.x));
    res = Vec3_add(res,
            Vec3_scale(camera -> defocus_disk_v, p.y));

    return res; 
}

static inline Ray get_ray(Camera* camera, int x, int y)
{
        Point3 pixel_center = camera -> pixel00_loc; 

        Vec3 offset = {random_double(-0.5, 0.5), random_double(-0.5, 0.5), 0};

        pixel_center = Vec3_add(pixel_center, Vec3_scale(camera -> pixel_delta_u, 
                    x + offset.x));
        pixel_center = Vec3_add(pixel_center, Vec3_scale(camera -> pixel_delta_v, 
                    y + offset.y));

        
        Point3 ray_orig = (camera -> defocus_angle <= 0) ? camera -> center : defocus_disk_sample(camera);

        Vec3 pixel_dir = Vec3_sub(pixel_center, ray_orig); 


        return (Ray){ray_orig, pixel_dir};

}

void *Camera_render_thread(void* rt_i)
{
    Render_thread_in* in = (Render_thread_in*) rt_i; 
    for (int j = 0; j < in -> end_y - in -> start_y; j++)
    {
       for (int i = 0; i < in -> end_x - in -> start_x; i++)
        {
            int x = i + in -> start_x; 
            int y = j + in -> start_y;  
            //printf("%d : %d %d\n", getpid(), x, y);
            Color pixel_color = {0, 0, 0}; 
            for (int sample = 0; sample < in -> camera -> samples_per_pixel; sample++)
            {
                Ray pixel_ray = get_ray(in -> camera, x, y);

                pixel_color = Vec3_add(pixel_color, Ray_color(pixel_ray, in -> hittables, 
                            in -> hittables_size, 
                            in -> camera -> max_depth)); 
            }

            Color color = Vec3_scale(pixel_color, in -> camera -> pixel_samples_scale); 
            libattopng_set_pixel(in -> png, i, j, color_rgb(color));
        }
    }

    return NULL; 
}

void Camera_render(Camera* camera, Hittable* hittables[], size_t hittables_size, int threads)
{

    libattopng_t* final_png = libattopng_new(camera -> image_width, 
          camera -> image_height, PNG_RGB); 

    libattopng_t* threads_png[threads]; 

    pthread_t threads_id[threads];
    Render_thread_in rt_i[threads];

    int y = 0; 
    int thread_y = (camera -> image_height) / threads; 

    for (int thread = 0; thread < threads - 1; thread++)
    {
        threads_png[thread] = libattopng_new(camera -> image_width, thread_y, PNG_RGB);
        
        Render_thread_in i = {
            .png = threads_png[thread],
            .camera = camera, 
            .hittables = hittables, 
            .hittables_size = hittables_size, 
            .start_x = 0, 
            .start_y = y, 
            .end_x = camera -> image_width, 
            .end_y = y + thread_y, 
        };

        rt_i[thread] = i; 
        
        pthread_create(&threads_id[thread], NULL, Camera_render_thread, (void*)&rt_i[thread]);

        y += thread_y; 
    }
    //last thread
    threads_png[threads - 1] = libattopng_new(camera -> image_width, 
            camera -> image_height - y, PNG_RGB);

    Render_thread_in i = {
        .png = threads_png[threads-1],
        .camera = camera, 
        .hittables = hittables, 
        .hittables_size = hittables_size, 
        .start_x = 0, 
        .start_y = y, 
        .end_x = camera -> image_width, 
        .end_y = camera -> image_height,  
    };

    rt_i[threads-1] = i; 
    
    pthread_create(&threads_id[threads-1], NULL, Camera_render_thread, (void*)&rt_i[threads-1]);

   for(int thread = 0; thread < threads; thread++)
   {
        pthread_join(threads_id[thread], NULL);
   }

    for (int y = 0; y < camera -> image_height; y++)
    {
        int png_id = MIN(y / thread_y, threads - 1);  
        int png_y = y - png_id * thread_y; 
        for (int x = 0; x < camera -> image_width; x++)
        {
            libattopng_set_pixel(final_png, x, y, libattopng_get_pixel(threads_png[png_id], x, png_y));
        }
    } 

    libattopng_save(final_png, "raytracing.png");
    for (int i = 0; i < threads; i++)
        libattopng_destroy(threads_png[i]);
    libattopng_destroy(final_png);
}
