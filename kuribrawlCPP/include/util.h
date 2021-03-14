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

    struct Rect {
        int w;
        int y;
    };

    struct VectorDouble {
        double x;
        double y;
    };
}

