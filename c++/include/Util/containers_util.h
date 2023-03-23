#include <map>

namespace Kuribrawl {
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