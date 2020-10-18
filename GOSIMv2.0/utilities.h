#ifndef UTILITIES_H_INCLUDED
#define UTILITIES_H_INCLUDED

#include <time.h>
#include <sys/time.h>
#include <cmath>

using namespace std;

template<typename T>
inline T max(const T &a, const T &b, const T &c) {
    if (a > b && a > c) { return a; }
    if (b > c) { return b; }
    return c;
}

template<typename T>
inline T min(const T &a, const T &b, const T &c) {
    if (a < b && a < c) { return a; }
    if (b < c) { return b; }
    return c;
}

inline int randomInt(int low, int high) {
    return (int)(((double)rand()/(RAND_MAX+1.0)) * (high - low + 1) + low);
}

inline float randomFloat(float low, float high) {
    return ((float)rand()/(RAND_MAX+1.0)) * (high - low) + low;
}

#endif // UTILITIES_H_INCLUDED
