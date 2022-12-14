#pragma once
#include <type_traits>

namespace Kuribrawl
{
    using namespace std;
    template<typename T>
    using bipair = pair<T, T>;

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

    template<typename T>
    struct Rect {
        T x;
        T y;
        T w;
        T h;

        inline const Vec2<T> asVec() const {
            return *((Vec2<T>*)this);
        }

        Rect(Vec2<T> position, SizeT<T> size) :
            x(position.x), y(position.y), w(size.w), h(size.h)
        {}

        Rect(Vec2<T> position, T w_, T h_) :
            x(position.x), y(position.y), w(w_), h(h_)
        {}
    };

    using Rectangle = Rect<int>;

    using VectorDouble = Vec2<double>;
} // namespace Kuribrawl
