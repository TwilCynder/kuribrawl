#include "Util/util.h"
#include "KBDebug/Debug.h"
#include <cmath>

using namespace Kuribrawl;

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

Kuribrawl::DirectionIG Kuribrawl::DirectionToDirectionIG(Kuribrawl::Direction direction, int facing){
    return (facing == -1 && (int)direction % 2 == 0) ? (DirectionIG)(2 - (int)direction) : (DirectionIG)direction;
}

Direction Kuribrawl::getDirection4(const Vector& stick_state, int threshold){
    int absX = abs(stick_state.x);
    int absY = abs(stick_state.y);

    if (absX < threshold && absY < threshold){
        return Direction::NONE;
    } else if (absX > absY){
        return (Direction)( 1 - sign(stick_state.x));
    } else {
        return (Direction)( 2 + sign(stick_state.y));
    }
}

DirectionIG Kuribrawl::getDirection4IG(const Vector& stick_state, int threshold, int facing) {
    int absX = abs(stick_state.x);
    int absY = abs(stick_state.y);

    if (absX < threshold && absY < threshold){
        return DirectionIG::NONE;
    } else if (absX > absY){
        return (DirectionIG)( 1 - (sign(stick_state.x) * facing));
    } else {
        return (DirectionIG)( 2 + sign(stick_state.y));
    }
}