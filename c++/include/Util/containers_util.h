#pragma once
#include <map>
#include <utility>

/**
 * @file containers_util.h
 * @brief Adds utility related to stl containers to the Kuribrawl namespace.  
 * Structures and functions related to the stl data structures go here, for completely new data structures see structures.h
 */

namespace Kuribrawl {
    /**
     * @brief Pair of references
     * @tparam T1 type of first member
     * @tparam T2 type of second member
     */
    template <typename T1, typename T2>
    using refpair_std = std::pair<T1&, T2&>;

    template <typename T1, typename T2>
    std::weak_ordering operator<=>(const std::pair<T1, T2>& p1, const refpair_std<T1, T2>& p2){
        auto res = p1.first <=> p2.second;
        if (res == 0) 
            return p1.second <=> p2.second;
        return res;    
    }

    template <typename T1, typename T2>
    bool operator==(const refpair_std<T1, T2>& p1, const refpair_std<T1, T2>& p2){
        return (p1.first == p2.first) && (p1.second == p2.second);
    }

    template <typename T1, typename T2>
    std::weak_ordering operator<=>(refpair_std<T1, T2>& p1, refpair_std<T1, T2>& p2){
        auto res = p1.first <=> p2.second;
        if (res == 0) 
            return p1.second <=> p2.second;
        return res;    
    }

    template <typename T1, typename T2>
    bool operator==(const std::pair<T1, T2>& p1, const refpair_std<T1, T2>& p2){
        return (p1.first == p2.first) && (p1.second == p2.second);
    }

    /**
     * @brief Pair of values of same type
     * @tparam T 
     */
    template<typename T>
    using bipair = std::pair<T, T>;

    template <typename T>
    using refbipair_std = refpair_std<T, T>;

    /**
     * @brief Returns a pointer the value associated with a given key in a map, or a null pointer.  
     * Only benefit of using this is that you dont have to get a pair iterator and then access its "value" attribute.  
     * @tparam K key type of the map
     * @tparam V value type of the map
     * @param map the map to search the key in
     * @param key 
     * @return const V*
     */
    template <typename K, typename V>
    const V* getMapValue(const std::map<K, V>& map, const K& key){
        auto it = map.find(key);
        if (it == map.end()){
            return nullptr;
        } else {
            return &it->second;
        }
    }
}