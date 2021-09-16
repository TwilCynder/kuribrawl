#pragma once

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

    struct Vector {
        int x;
        int y;
    };

    template <typename T>
    struct VectorT {
        T x;
        T y;
    };

    struct Rect {
        int w;
        int y;
    };

    struct VectorDouble {
        double x;
        double y;
    };

    signed char sign(int);
    signed char sign(double);
    signed char signReduced(int value, int minimum);
    void substractValue(double* v1, double v2);
}

