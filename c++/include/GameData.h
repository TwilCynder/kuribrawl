#pragma once
#include <map>
#include <string>
#include "Champion.h"

class App;

/**
 * @brief Class containing all data related to gameplay.
 * This includes every Champion, Stage, etc
 */
class GameData {
    public:

    Champion* getChampion(const char* name);
    Champion* addChampion(const char* name);
    Champion& tryChampion(const char* name);

    void readDataFile();
    void finalizeChampionsInitialization();

    private:
    using ChampionsMap = std::map<std::string, Champion>;
    ChampionsMap champions; ///< Map containing all Champions and associating them with a string ID (= name).
};