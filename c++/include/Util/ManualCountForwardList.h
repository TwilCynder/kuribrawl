#include <forward_list>

/**
 * @brief A forward list with a size counter that you need to increase or decrease yourself.
 * I swear it's useful
 * @tparam T 
 * @tparam Allocator 
 */
template<
    class T,
    class Allocator = std::allocator<T>
>
class ManualCountForwardList : public std::forward_list<T, Allocator>{
    using std::forward_list<T, Allocator>::forward_list;

    int count = 0;

    public:

    /* ça c'est à quoi ça ressemblerait si j'implémentais vraiment le counter
    template <typename... Args>
    auto clear(Args... args){
        size = 0;
        return std::forward_list<T, Allocator>::clear();
    }
    */

    inline void operator++(int){
        count++;
    }

    inline void operator--(int){
        count--;
    }

    int size(){
        return count;
    }

};