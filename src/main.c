#include <stdio.h> 

#include "material.h"
#include "camera.h"
#include "hittable.h"
#include "interval.h"
#include "random.h"

int main(void)
{
    srand(time(NULL));

    Camera camera; 

    Camera_init(&camera);
    Hittable* hittables[MAX_HITTABLES] = {0};
    Sphere sphere[MAX_HITTABLES] = {0};
    Material  materials[MAX_HITTABLES] = {0};

    int i = 0; 

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            int random_mat = rand() % 20; 
            Point3 center = {a + random_double(0, 0.9), 0.2, b + random_double(0, 0.9)}; 

            if (Vec3_len(Vec3_sub(camera.center, (Point3){4, 0.2, 0})) > 0.9)
                {
                if (random_mat < 15) {
                    // diffuse
                    Color albedo = Vec3_mul(Vec3_rand(0.0, 1.0), Vec3_rand(0.0, 1.0)); 
                    Lambertian_init(&materials[i], albedo);
                    Sphere_init(&sphere[i], center, 0.2, &materials[i]);
                    hittables[i] = (Hittable*) &sphere[i];
                    i++; 
                } else if (random_mat < 19) {
                    // metal
                    Color albedo = Vec3_rand(0.5, 1);
                    double fuzz = random_double(0, 0.5);
                    Metal_init(&materials[i], albedo, fuzz);
                    Sphere_init(&sphere[i], center, 0.2, &materials[i]);
                    hittables[i] = (Hittable*) &sphere[i];
                    i++; 
                } else {
                    // glass
                    Dielectric_init(&materials[i], 1.5);
                    Sphere_init(&sphere[i], center, 0.2, &materials[i]);
                    hittables[i] = (Hittable*) &sphere[i];
                    i++; 
                }
            }
        }
    }

    Material ground; 
    Lambertian_init(&ground, (Color){0.5, 0.5, 0.5});

    Sphere ground_sphere; 
    Sphere_init(&ground_sphere, (Point3){0, -1000, 0}, 1000, &ground);
    hittables[i++] = (Hittable*) &ground_sphere;

    Material material1; 
    Dielectric_init(&material1, 1.5);

    Sphere sphere1; 
    Sphere_init(&sphere1, (Point3){0, 1, 0}, 1.0, &material1);
    hittables[i++] = (Hittable*) &sphere1;
    
    Material material2; 
    Lambertian_init(&material2, (Color){0.4, 0.2, 0.1});

    Sphere sphere2; 
    Sphere_init(&sphere2, (Point3){-4, 1, 0}, 1.0, &material2);
    hittables[i++] = (Hittable*) &sphere2;

    Material material3; 
    Metal_init(&material3, (Color){0.7, 0.6, 0.5}, 0.0);

    Sphere sphere3; 
    Sphere_init(&sphere3, (Point3){4, 1, 0}, 1.0, &material3);
    hittables[i++] = (Hittable*) &sphere3;

    Camera_render(&camera, hittables, i, THREADS);


    return 0;
}

