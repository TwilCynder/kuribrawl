#pragma once

#include <ostream>

template <class T, std::ostream&(T::*F)(std::ostream&)>
class LogWrapper {
    T* obj;

    public:

    LogWrapper(T* obj_): obj(obj_){}

    std::ostream& operator()(std::ostream& os) const {
        return (obj->*F)(os);
    }
};

template <class T>
auto lmf_wrap(T* obj, std::ostream&(T::*F)(std::ostream&)){
    return LogWrapper<T, F>(obj);
}