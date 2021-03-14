#include "GameData.h"
#include "Debug.h"

Champion* GameData::getChampion(const std::string& name){
    auto it = champions.find(name);
    if (it == champions.end()){
      return NULL;
    }
    return &(it->second);
}

Champion* GameData::addChampion(const std::string& name) {
    auto [node, success] = champions.try_emplace(name, name);

    if (!success) {
        throw KBFatal("Could not create champion");
    }

    Champion& champion = node->second;
    return &champion;
}