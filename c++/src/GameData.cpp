#include <memory>
#include "GameData.h"
#include "KBDebug/Debug.h"

/**
 * @brief Returns the Champion associated with the given name.
 *
 * @param name the string identifier of the wanted Champion
 * @return Champion* the Champion, or NULL if no Champion had this name.
 */
Champion* GameData::getChampion(const char* name){
    auto it = champions.find(name);
    if (it == champions.end()){
      return NULL;
    }
    return &(it->second);
}

/**
 * @brief Adds a new Champion to the GameData.
 * Contructs the Champion in-place.
 * @param name the name of the new Champion ; will be both its key in the GameData and its \ref Champion#name "name attribute".
 * @return Champion* the created Champion (can't be NULL since the method throws if it could be created)
 */
Champion* GameData::addChampion(const char* name) {
    auto [node, success] = champions.try_emplace(name, name);
    if (!success) {
        throw KBFatal("Could not create champion");
    }

    Champion& champion = node->second;
    return &champion;
}

/**
 * @brief Returns a Champion associated with the given name, creating it if there was none.
 *
 * @param name the name of the wanted Champion.
 * @return Champion& a reference to the Champion.
 */
Champion& GameData::tryChampion(const char* name){
    auto [node, success] = champions.try_emplace(name, name);
    return node->second;
}

void GameData::finalizeChampionsInitialization(){
    ChampionsMap::iterator it;
    for (it = champions.begin(); it != champions.end(); it++)
    {
        it->second.initDefaultAnimations();
        it->second.finalizeMoves();
    }
}