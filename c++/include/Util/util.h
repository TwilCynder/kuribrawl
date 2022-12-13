#pragma once

#include <type_traits>

#define PI 3.1416
#define addBitValue(value, shift) + (value << shift)
#define getBits(value, mask, shift) ((value & mask) >> shift)
#define getBit(value, shift) ((value & (1 << shift) )  >> shift )
#define checkNull(value, exception) if (!value) {throw exception;}

namespace Kuribrawl {
    using namespace std;
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

    template <typename T, typename Enabler = void>
    struct Vec2 {
        T x;
        T y;
    };

    using Vector = Vec2<int>;
    
    template <typename T>
    struct Vec2 <T, enable_if_t<is_class_v<T> && !is_aggregate_v<T>>> {
        T x;
        T y;

        /*
        template< class... Args1, class... Args2 >
        Vec2( std::piecewise_construct_t t,
            std::tuple<Args1...> first_args,
            std::tuple<Args2...> second_args ) :
            x(first_args),
            y(second_args)
        {}
        */

        Vec2(T&& x_, T&& y_) : x(move(x_)), y(move(y_))
        {}

        Vec2()
        {}
    };

    template<typename T>
    struct ArithVec2 : public Vec2<T> {
        template<typename U>
        ArithVec2<U> operator+(ArithVec2<U>&& other){
            return ArithVec2(this->x + other.x, this->y + other.y);
        }

        template<typename U>
        ArithVec2<U> operator+(U&& other){
            return ArithVec2(this->x + other, this->y + other);
        }
    };

    template<typename T>
    struct SizeT {
        T w;
        T h;
    };
    using Size = SizeT<int>;

    


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
}

