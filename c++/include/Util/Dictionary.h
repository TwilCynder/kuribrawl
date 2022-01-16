#pragma once
#include <exception>
#include <map>

template <typename T>
class Dictionnary {
    public:
    std::map<std::string, T> elements;

    public:
    bool add(const char* name, T element){
        auto [node, success]  = elements.emplace(name, element);
        return success;
    }
    const T get(const std::string& name) const {
        auto it = elements.find(name);
        if (it == elements.end()) return NULL;
        return it->second;
    }
    const T get(const char* name) const {
        auto it = elements.find(name);
        if (it == elements.end()) return NULL;
        return it->second;
    };
};