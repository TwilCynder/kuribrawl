#pragma once

#include "structures.h"

#define PI 3.1416
#define addBitValue(value, shift) + (value << shift)
#define getBits(value, mask, shift) ((value & mask) >> shift)
#define getBit(value, shift) ((value & (1 << shift) )  >> shift )
#define checkNull(value, exception) if (!value) {throw exception;}

namespace Kuribrawl {
    enum class Direction {
        RIGHT,
        UP,
        LEFT,
        DOWN,
        NONE
    };

    enum class DirectionIG {
        FORWARD,
        UP,
        BACK,
        DOWN,
        NONE
    };


    signed char sign(int);
    signed char sign(double);
    signed char signReduced(int value, int minimum);
    void substractValue(double* v1, double v2);

    template<typename T>
    T min (T v1, T v2){
        return (v1 > v2) ? v2 : v1;
    }

    template<typename T>
    T max (T v1, T v2){
        return (v1 < v2) ? v2 : v1;
    }

    DirectionIG DirectionToDirectionIG(Direction, int facing);
    Direction getDirection4(const Vector& stick_state, int threshold);
    DirectionIG getDirection4IG(const Vector& stick_state, int threshold, int facing);

    bool isRight(const Kuribrawl::Vector& stick, int threshold);
    bool isUp(const Kuribrawl::Vector& stick, int threshold);
    bool isLeft(const Kuribrawl::Vector& stick, int threshold);
    bool isDown(const Kuribrawl::Vector& stick, int threshold);
}

