#include "Util/util.h"
#include "Util/std_util.h"
#include "KBDebug/Debug.h"
#include <cmath>
#include <compare>

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

Kuribrawl::Side Kuribrawl::operator-(Kuribrawl::Side s)
{
    return Kuribrawl::oppSide(s);
}

bool Kuribrawl::operator!(Kuribrawl::Side s)
{
    return !(int8_t)s;
}

Kuribrawl::DirectionIG Kuribrawl::DirectionToDirectionIG(Kuribrawl::Direction direction, Kuribrawl::Side facing){
    return (facing == Kuribrawl::Side::LEFT && (direction == Direction::LEFT || direction == Direction::RIGHT)) ? (DirectionIG)(2 - (int)direction) : (DirectionIG)direction;
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

DirectionIG Kuribrawl::getDirection4IG(const Vector& stick_state, int threshold, Kuribrawl::Side facing) {
    int absX = abs(stick_state.x);
    int absY = abs(stick_state.y);

    if (absX < threshold && absY < threshold){
        return DirectionIG::NONE;
    } else if (absX > absY){
        return (DirectionIG)( 1 - (sign(stick_state.x) * (int8_t)facing));
    } else {
        return (DirectionIG)( 2 + sign(stick_state.y));
    }
}

bool Kuribrawl::isRight(const Kuribrawl::Vector& stick, int threshold){
    return stick.x > threshold;
}

bool Kuribrawl::isUp(const Kuribrawl::Vector& stick, int threshold){
    return stick.y < -threshold;
}

bool Kuribrawl::isLeft(const Kuribrawl::Vector& stick, int threshold){
    return stick.x < -threshold;
}

bool Kuribrawl::isDown(const Kuribrawl::Vector& stick, int threshold){
    return stick.y > threshold;
}

inline std::weak_ordering partial_to_weak(std::partial_ordering& order){
    return 
        order == std::partial_ordering::less ? std::weak_ordering::less : 
        order == std::partial_ordering::equivalent ? std::weak_ordering::equivalent : 
        order == std::partial_ordering::greater ? std::weak_ordering::greater : 
        throw;
}

std::weak_ordering Kuribrawl::cmp(double a, double b){
    std::partial_ordering p_order = a <=> b;
    return (p_order != std::partial_ordering::unordered) ? partial_to_weak(p_order) : 
        std::isnan(a) ? 
            std::isnan(b) ? std::weak_ordering::equivalent : std::weak_ordering::less :
            std::isnan(b) ? std::weak_ordering::greater : throw;

}