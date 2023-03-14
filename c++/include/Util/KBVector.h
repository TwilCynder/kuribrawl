#pragma once
#include <vector>

/**
 * @brief std::vector expanded with some utility methods
 * 
 * @tparam T 
 */
template<typename T>
class KBVector : public std::vector<T>{
    void fill(T&& value){
        for (int i = 0; i < size(); i++){
            this[i] = value;
        }
    }
};