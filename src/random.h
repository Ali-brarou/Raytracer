#ifndef RANDOM_H
#define RANDOM_H

#include <stdlib.h>  // rand() and RAND_MAX
#include <time.h> 

inline double random_double(double min, double max)
{
    return min + (max - min) * (rand() / (RAND_MAX + 1.0));
}

#endif 
