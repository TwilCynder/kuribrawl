#pragma once
#include <exception>
#include <map>
#include "stringOperations.h"


/**
 * @brief A Dictionnary, mapping a name to a value. 
 * @tparam T type of the values  
 * @tparam null null element of type T, defaults to T{}
 */
template <typename T, T null = T{}>
class Dictionnary {
    public:
    /// Map containing the names and elements.  
    std::map<std::string, T, std::less<>> elements;

    public:
    /**
     * @brief returns the value associated with the given name or null
     * @param name key
     * @return const T a value of type t ; can be the "null" template param
     */
    const T value(const std::string& name) const {
        auto it = elements.find(name);
        if (it == elements.end()) return null;
        return it->second;
    }
    /**
     * @brief returns the value associated with the given name or null
     * @param name key
     * @return const T a value of type t ; can be the "null" template param
     */
    const T value(const char* name) const {
        auto it = elements.find(name);
        if (it == elements.end()) return null;
        return it->second;
    }
    /**
     * @brief returns the value associated with the given name or null
     * @param name key
     * @return const T a value of type t ; can be the "null" template param
     */
    const T value(const Kuribrawl::string_view& name) const {
        auto it = elements.find(name);
        if (it == elements.end()) return null;
        return it->second;
    }
    
    /**
     * @brief associates a value to a name if it wasn't already in the dictionnary 
     * @param name key
     * @param element value
     * @return bool whether the insertion was successful (name didn't already exist)
     */
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


    /**
     * @brief Returns the value associated with a given name, creating it if it wasn't in the dictionnary.  
     * This implies default-initializing the value, which needs to be possible.
     * @param name key
     * @return T& existing value associated to name or newly default-constructed value
     */
    /*
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

*/

    /**
     * @brief Returns the value associated with a given name, creating it with the given arguments if it wasn't in the dictionnary
     * @tparam Args arguments for a T constructor  
     * @param name key 
     * @param args arguments passed to the value constructor if the element doesn't exist yet.  
     * @return T& existing value associated to name or newly-constructed value
     */
    template <class... Args>
    T& get(const std::string& name, Args&&... args){
        auto [node, success] = elements.try_emplace(name, args...);
        return node->second;
    }
    template <class... Args>
    T& get(const std::string&& name, Args&&... args){
        auto [node, success] = elements.try_emplace(std::move(name), args...);
        return node->second;
    }
    template <class... Args>
    T& get(const char* name, Args&&... args){
        return get((std::string)name, args...);
    }    
    template <class... Args>
    T& get(const Kuribrawl::string_view name, Args&&... args){
        return get((std::string)name, args...);
    }

    /**
     * @brief Associates a value to the given key, even if the key was already present.  
     * @tparam Args 
     * @param name 
     * @param obj 
     * @return T& 
     */
    T& set(const std::string&& name, T&& obj){
        auto [node, success] = elements.insert_or_assign(name, obj);
        return node->second;
    }
    T& set(const char* name, T&& obj){
        return set((std::string)name, obj);
    }
    T& set(const Kuribrawl::string_view name, T&& obj){
        return set((std::string)name, obj);
    }

    template<typename A>
    T& operator[](A name){
        return get(name);
    }

};