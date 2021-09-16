#include "util.h"
#include "Debug.h"
#include <cmath>

signed char Kuribrawl::sign(int v) {
    return (0 < v) - (v < 0);
}

signed char Kuribrawl::sign(double v){
    return (0 < v) - (v < 0);
}

signed char Kuribrawl::signReduced(int v, int m){
    return (v < -m) ? -1 : (v > m) ? 1 : 0;
}

void Kuribrawl::substractValue(double* v1, double v2){
    int s = sign(*v1);
    *v1 = abs(*v1);
    *v1 -= v2;
    if (*v1 < 0) *v1 = 0;
    *v1 *= s;
}