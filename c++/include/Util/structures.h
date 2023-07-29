#pragma once
#include <type_traits>
#include <utility>
#include <ostream>

/**
 * @file structures.h
 * @brief Defines data structures for the Kuribrawl namespace.  
 * Entirely new data structures go here, for extension of stl data structures sett containers_util.h.  
 */

/**
 * @brief namespace used for general utility features.  
 */

namespace Kuribrawl
{
    template <typename T, typename U>
    concept SumExists = requires (T t, U u){
        t + u;
    };
    template <typename T, typename U>
    concept SubstExists = requires (T t, U u){
        t - u;
    };
    template <typename T, typename U>
    concept MultExists = requires (T t, U u){
        t * u;
    };
    template <typename T, typename U>
    concept DivExists = requires (T t, U u){
        t / u;
    };

    template <typename T>
    concept NotAnAggregate = !std::is_aggregate_v<T>;


    template <typename T, typename Enabler = void>
    struct Vec2 {
        union {
            T x; 
            T left; 
        };
        union {
            T y;
            T right;
        };

        void set(T x_, T y_){
            x = x_;
            y = y_;
        }
    };

    template <typename T>
    struct Vec2 <T, std::enable_if_t<std::is_class_v<T> && !std::is_aggregate_v<T>>> {
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

        Vec2(T&& x_, T&& y_) : x(std::move(x_)), y(std::move(y_))
        {}

        Vec2(const T& x_, const T& y_) : x(x_), y(y_)
        {}

        Vec2()
        {}

    };

    template<typename T>
    struct ArithVec2 : public Vec2<T> {
        ArithVec2<T>& operator=(const Vec2<T>& v){
            this->x = v.x;
            this->y = v.y;
        }

        template<typename U>
        ArithVec2<T>& operator=(const Vec2<U>& v){
            this->x = v.x;
            this->y = v.y;
            return *this;
        }

        template<typename U>
            requires SumExists<T, U>
        auto operator+(const ArithVec2<U>& other){
            return ArithVec2(this->x + other.x, this->y + other.y);
        }

        template<typename U>
            requires SumExists<T, U>
        auto operator+(const U& other){
            return ArithVec2(this->x + other, this->y + other);
        }

        ArithVec2<T>& operator+=(const Vec2<T>& v){
            this->x += v.x;
            this->y += v.y;
            return *this;
        }

        template<typename U>
        ArithVec2<T>& operator+=(const U& v){
            this->x += v;
            this->y += v;
            return *this;
        }

        template<typename U>
            requires SubstExists<T, U>
        auto operator-(const Vec2<U>& other){
            return ArithVec2{this->x - other.x, this->y - other.y};
        }

        template<typename U>
            requires SubstExists<T, U>
        auto operator-(const U& other){
            return ArithVec2(this->x - other, this->y - other);
        }

        ArithVec2<T>& operator-=(const Vec2<T>& v){
            this->x -= v.x;
            this->y -= v.y;
            return *this;
        }

        template<typename U>
        ArithVec2<T>& operator-=(const U& v){
            this->x -= v;
            this->y -= v;
            return *this;
        }

        template<typename U>
            requires MultExists<T, U>
        auto operator*(const ArithVec2<U>& other){
            return ArithVec2(this->x * other.x, this->y * other.y);
        }

        template<typename U>
            requires MultExists<T, U>
        auto operator*(const U& other){
            return ArithVec2(this->x * other, this->y * other);
        }

        ArithVec2<T>& operator*=(const Vec2<T>& v){
            this->x *= v.x;
            this->y *= v.y;
            return *this;
        }

        template<typename U>
        ArithVec2<T>& operator*=(const U& v){
            this->x *= v;
            this->y *= v;
            return *this;
        }

        template<typename U>
            requires DivExists<T, U>
        auto operator/(const ArithVec2<U>& other){
            return ArithVec2(this->x / other.x, this->y / other.y);
        }

        template<typename U>
            requires DivExists<T, U>
        auto operator/(const U& other){
            return ArithVec2(this->x / other, this->y / other);
        }

        ArithVec2<T>& operator/=(const Vec2<T>& v){
            this->x /= v.x;
            this->y /= v.y;
            return *this;
        }

        template<typename U>
        ArithVec2<T>& operator/=(const U& v){
            this->x /= v;
            this->y /= v;
            return *this;
        }

        T normSquare(){
            return this->x * this->x + this->y * this->y;
        }

        T norm(){
            return sqrt(normSquare);
        }

    };

    template <typename T>
    std::ostream& operator<<(std::ostream& os, Vec2<T>& vec){
        return os << vec.left << ", " << vec.right;
    }
    
    using Vector = ArithVec2<int>;

    template<typename T>
    struct SizeT {
        T w;
        T h;
    };
    using Size = SizeT<int>;

    template <typename T>
    struct Rect {
                union {
            T x;
            T left;
        };
        union {
            T y;
            T top;
        };
        union {
            T w;
            T right;
        };
        union {
            T h;
            T bottom;
        };

        inline const Vec2<T> asVec() const {
            return *((Vec2<T>*)this);
        }
    };

    template<typename T>
    requires (std::is_class_v<T> && !std::is_aggregate_v<T>)
    struct Rect <T> {
        union {
            T x;
            T left;
        };
        union {
            T y;
            T top;
        };
        union {
            T w;
            T right;
        };
        union {
            T h;
            T bottom;
        };

        inline const Vec2<T> asVec() const {
            return *((Vec2<T>*)this);
        }

        constexpr Rect(T x_, T y_, T w_, T h_) : 
            x(x_), y(y_), w(w_), h(h_)
        {
            //#pragma message "Coucou je suis la !"
        }

        constexpr Rect(Vec2<T> position, SizeT<T> size) :
            x(position.x), y(position.y), w(size.w), h(size.h)
        {}

        constexpr Rect(Vec2<T> position, T w_, T h_) :
            x(position.x), y(position.y), w(w_), h(h_)
        {}

    };

    template <typename T>
    struct ArithRect : public Rect<T> {
        ArithRect<T> operator+ (const Vec2<T>& vec){
            return Rect<T>(this->x + vec.x, this->y + vec.y, this->w, this->h);
        }

        template<typename U>
        ArithRect<T> operator*(const U& coef){
            return Rect<T>(this->x, this->y, this->w * coef, this->h * coef);
        }
    };

    using Rectangle = Rect<int>;

    //TODO arihvec2
    using VectorDouble = Vec2<double>;
    
    template <class T1, class T2> 
    struct refpair {
        T1& first;
        T2& second;

        refpair(T1& x, T2& y) : 
            first(x), second(y) {}

        template <class U, class V>
        refpair<T1,T2>& operator=(const std::pair<U,V> &p){
            first=p.first;
            second=p.second;
            return *this;
        }

        std::weak_ordering operator<=>(const std::pair<T1, T2>& other) const {
            auto res = first <=> other.first;
            if (res == 0) 
                return second <=> other.second;
            return res; 
        }

        template <typename U1, typename U2>
        std::weak_ordering operator<=>(const std::pair<U1, U2>& other) const {
            auto res = first <=> other.first;
            if (res == 0) 
                return second <=> other.second;
            return res; 
        }

        bool operator==(const std::pair<T1, T2>& other) const {
            return (first == other.first) && (second == other.second); 
        }

        template <typename U1, typename U2>
        bool operator==(const std::pair<U1, U2>& other) const {
            return (first == other.first) && (second == other.second); 
        }
    };

    template <typename T>
    using refbipair = refpair<T, T>;

} // namespace Kuribrawl
