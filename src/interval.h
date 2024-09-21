#ifndef INTERVAL_H
#define INTERVAL_H

#include <math.h> 
#include <sys/param.h> 
#include <stdbool.h> 

#define CLAMP(X, min, max) \
    MAX(MIN(X, max), min)

#define INTERVAL_EMPTY (Interval){+INFINITY, -INFINITY}
#define INTERVAL_UNIVERSE (Interval){-INFINITY, +INFINITY}

typedef struct {
    double min; 
    double max; 
} Interval; 

inline double Interval_size(Interval interval)
{
    return interval.max - interval.min;
}

inline bool Interval_contains(Interval interval, double x)
{
    return interval.min <= x && x <= interval.max; 
}

inline bool Interval_surrounds(Interval interval, double x)
{
    return interval.min < x && x < interval.max; 
}



#endif
