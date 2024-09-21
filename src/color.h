#ifndef COLOR_H
#define COLOR_H

#include <stdio.h> 
#include <stdint.h> 

#include "vector3.h"
#include "interval.h"

//some common colors
#define RED     (Color){1.0, 0.0, 0.0}
#define GREEN   (Color){0.0, 1.0, 0.0}
#define BLUE    (Color){0.0, 0.0, 1.0}
#define WHITE   (Color){1.0, 1.0, 1.0}
#define BLACK   (Color){0.0, 0.0, 0.0}

typedef Vec3 Color; 

inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
        return sqrt(linear_component);

    return 0;
}

inline void color_write(const Color color)
{
    double r = color.x; 
    double g = color.y; 
    double b = color.z; 

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    int rbyte = 255.999 * CLAMP(r, 0.0, 0.999);  
    int gbyte = 255.999 * CLAMP(g, 0.0, 0.999);  
    int bbyte = 255.999 * CLAMP(b, 0.0, 0.999);  

    printf("%d %d %d\n", rbyte, gbyte, bbyte);
}

inline uint32_t color_rgb(const Color color)
{
    double r = color.x; 
    double g = color.y; 
    double b = color.z; 

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    int rbyte = 255.999 * CLAMP(r, 0.0, 0.999);  
    int gbyte = 255.999 * CLAMP(g, 0.0, 0.999);  
    int bbyte = 255.999 * CLAMP(b, 0.0, 0.999);  

    return ((rbyte) | (gbyte << 8) | (bbyte << 16)) & 0xFFFFFF; 
}

#endif
