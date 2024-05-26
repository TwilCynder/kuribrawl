#pragma once

#include <ostream>


template <class T, std::ostream&(T::*F)(std::ostream&) >
class LogWrapper {
    T* obj;

    public:

    LogWrapper(T* obj_): obj(obj_){}

    std::ostream& operator()(std::ostream& os) const {
        return (obj->*F)(os);
    }
};

/*
template <class T>
constexpr auto lmf_wrap(T* obj, std::ostream&(T::*F)(std::ostream&) ){
    return LogWrapper<T, F>(obj);
}*/


template <class T, std::ostream&(T::*F)(std::ostream&) const>
class ConstLogWrapper {
    const T* obj;

    public:

    ConstLogWrapper(const T* obj_): obj(obj_){}

    std::ostream& operator()(std::ostream& os) const {
        return (obj->*F)(os);
    }
};

/*
template <class T>
auto lmf_wrap(T* obj, std::ostream&(T::*F)(std::ostream&) const){
    return LogWrapper<T, F>(obj);
}
*/


#define LMFWRAP(cl, method, obj) LogWrapper<cl, method>(obj)
#define CLMFWRAP(cl, method, obj) ConstLogWrapper<cl, method>(obj)