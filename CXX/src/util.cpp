#include "util.h"
#include "Debug.h"
#include <cmath>

int Kuribrawl::sign(int v) {
    return (0 < v) - (v < 0);
}

int Kuribrawl::sign(double v){
    return (0 < v) - (v < 0);
}

void Kuribrawl::substractValue(double* v1, double v2){
    int s = sign(*v1);
    *v1 = abs(*v1);
    *v1 -= v2;
    if (*v1 < 0) *v1 = 0;
    *v1 *= s;
}