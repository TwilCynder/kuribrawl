#pragma once
#include <exception>
#include <map>
#include "stringOperations.h"

template <typename T, T null = T{}>
class Dictionnary {
    public:
    std::map<std::string, T, std::less<>> elements;

    public:
    const T value(const std::string& name) const {
        auto it = elements.find(name);
        if (it == elements.end()) return NULL;
        return it->second;
    }
    const T value(const char* name) const {
        auto it = elements.find(name);
        if (it == elements.end()) return NULL;
        return it->second;
    }
    const T value(const Kuribrawl::string_view& name) const {
        auto it = elements.find(name);
        if (it == elements.end()) return NULL;
        return it->second;
    }
    bool add(const char* name, T element){
        auto [node, success]  = elements.emplace(name, element);
        return success;
    }
    bool add(std::string&& name, T element){
        auto [node, success]  = elements.emplace(std::move(name), element);
        return success;
    }
    bool add(std::string& name, T element){
        auto [node, success]  = elements.emplace(name, element);
        return success;
    }
    T& get(const std::string& name) {
        auto [node, success]  = elements.try_emplace(name);
        return node->second;
    }
    T& get(std::string&& name) {
        auto [node, success]  = elements.try_emplace(std::move(name));
        return node->second;
    }
    T& get(const char* name){
        return get((std::string)name);
    }
    T& get(const Kuribrawl::string_view name){
        return get((std::string)name);
    }
    template<typename A>
    T& operator[](A name){
        return get(name);
    }

};