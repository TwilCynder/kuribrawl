#pragma once

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

    struct Vector {
        int x;
        int y;
    };

    template <typename T>
    struct VectorT {
        T x;
        T y;
        template< class... Args1, class... Args2 >
        VectorT( std::piecewise_construct_t t,
            std::tuple<Args1...> first_args,
            std::tuple<Args2...> second_args ) :
            x(first_args),
            y(second_args)
        {}

        VectorT(){}
    };

    struct Rect {
        int w;
        int h;
    };

    struct VectorDouble {
        double x;
        double y;
    };

    template<typename T>
    struct SideH {
        T left;
        T right;
    };

    using SideHInt = SideH<int>;

    signed char sign(int);
    signed char sign(double);
    signed char signReduced(int value, int minimum);
    void substractValue(double* v1, double v2);
    DirectionIG DirectionToDirectionIG(Direction, int facing);

    Direction getDirection4(const Vector& stick_state, int threshold);
    DirectionIG getDirection4IG(const Vector& stick_state, int threshold, int facing);
}

