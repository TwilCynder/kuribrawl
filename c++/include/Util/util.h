#pragma once

#include <cstdint>
#include "structures.h"

#define PI 3.1416
#define addBitValue(value, shift) + (value << shift)
#define getBits(value, mask, shift) ((value & mask) >> shift)
#define getBit(value, shift) ((value & (1 << shift) )  >> shift )
#define checkNull(value, exception) if (!value) {throw exception;}

namespace Kuribrawl {
    template<typename T>
    T min (T v1, T v2){
        return (v1 > v2) ? v2 : v1;
    }

    template<typename T>
    T max (T v1, T v2){
        return (v1 < v2) ? v2 : v1;
    }

    //ajouter NEUTRAL (actuellement NONE est utilisé, mais mtn NONE devrait vouloir dire non-spécifié)
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

    enum class Side {
        LEFT = -1,
        NEUTRAL = 0,
        RIGHT = 1
    };

    signed char sign(int);
    signed char sign(double);
    signed char signReduced(int value, int minimum);
    void substractValue(double* v1, double v2);

    template <typename T>
    inline Kuribrawl::Side signToSide(T v){
        return (Kuribrawl::Side)sign(v);
    }

    template <typename T>
    inline T flipValue(T v, Kuribrawl::Side side){
        return v * (int8_t)side;
    }

    Side oppSide(Side side);

    Kuribrawl::Side operator-(Kuribrawl::Side);
    bool operator!(Kuribrawl::Side);

    DirectionIG DirectionToDirectionIG(Direction, Side facing);
    Direction getDirection4(const Vector& stick_state, int threshold);
    DirectionIG getDirection4IG(const Vector& stick_state, int threshold, Side facing);

    bool isRight(const Kuribrawl::Vector& stick, int threshold);
    bool isUp(const Kuribrawl::Vector& stick, int threshold);
    bool isLeft(const Kuribrawl::Vector& stick, int threshold);
    bool isDown(const Kuribrawl::Vector& stick, int threshold);

    //FROM CHATGPT. FUCK. I HAD A GOOD 70% OF THE SOLUTION MYSELF BTW BTW.
    template<typename Range, typename T>
    concept RangeOf = requires(Range r) {
        typename Range::value_type; // Check if the range has a nested 'value_type'
        requires std::same_as<typename Range::value_type, T>; // Ensure the value_type is T
        { std::begin(r) } -> std::same_as<typename Range::iterator>; // Check begin() returns the correct iterator type
        { std::end(r) } -> std::same_as<typename Range::iterator>;   // Check end() returns the correct iterator type
    };

    template <typename T, typename Range, typename U = T>
    requires RangeOf<Range, U>
    T average(const Range& r, int size){
        T res{};
        for (U el : r){
            res += el;
        }
        res /= size;
        return res;
    }

}