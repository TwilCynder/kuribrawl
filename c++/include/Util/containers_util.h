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
    using refpair = std::pair<T1&, T2&>;
  

    template <typename T1, typename T2>
    std::weak_ordering operator<=>(const std::pair<T1, T2>& p1, const refpair<T1, T2>& p2){
        auto res = p1.first <=> p2.second;
        if (res == 0) 
            return p1.second <=> p2.second;
        return res;    
    }

    template <typename T1, typename T2>
    int operator==(const std::pair<T1, T2>& p1, const refpair<T1, T2>& p2){
        return (p1.first == p2.first) && (p1.second == p2.second);
    }

    /**
     * @brief Pair of values of same type
     * @tparam T 
     */
    template<typename T>
    using bipair = std::pair<T, T>;

    template <typename T>
    using refbipair = refpair<T, T>;

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

    template<typename K, typename V>
    class DynamicMatrix : public std::map<bipair<K>, V, std::less<>>{
        public:
        using ref_key = refbipair<K>;

        template<class... Args>
        void try_emplace(const K& k1, const K& k2, Args&&... args){
            try_emplace(std::pair<K, K>(k1, k2), args...);
        }
        template<class... Args>
        void try_emplace(K&& k1, K&& k2, Args&&... args){
            try_emplace(std::pair<K, K>(k1, k2), args...);
        }
    };
}