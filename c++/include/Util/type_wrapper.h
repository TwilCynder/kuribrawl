template <typename T, typename tag>
struct type_wrapper {
    T v;

    explicit type_wrapper(T&& v_) :
        v(v_) 
    {}

    explicit type_wrapper(T& v_) : 
        v(v_) 
    {}
};