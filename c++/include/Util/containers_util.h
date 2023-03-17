#include <map>

namespace Kuribrawl {
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