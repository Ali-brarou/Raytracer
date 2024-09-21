#ifndef CONFIG_H
#define CONFIG_H

#define IMAGE_WIDTH 100 //3840//4k resolution
#define ASPECT_RATIO (16.0 / 9.0)
#define VFOV 20
#define LOOKFROM (Point3){13, 2, 3}
#define LOOKAT   (Point3){0, 0, 0}
#define VUP      (Vec3){0, 1, 0}
#define DEFOCUS_ANGLE 0.6
#define FOCUS_DIS 10.0

#define MAX_HITTABLES 488

#define SAMPLES_PER_PIXEL 10
#define MAX_DEPTH 50

#define THREADS 12

#endif
