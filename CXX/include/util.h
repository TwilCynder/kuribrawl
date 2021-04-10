#pragma once

namespace Kuribrawl {
    enum Direction {
        RIGHT,
        UP,
        LEFT,
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

    int sign(int);
    int sign(double);
    void substractValue(double* v1, double v2);
}

