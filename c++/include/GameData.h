#pragma once
#include <map>
#include <string>
#include "Champion.h"
#include "Stage.h"

class App;

/**
 * @brief Class containing all data related to gameplay.
 * This includes every Champion, Stage, etc
 */
class GameData {
    public:

    Champion* getChampion(const char* name);
    Champion& addChampion(const std::string& name);
    Champion& addChampion(std::string&& name);
    Champion& tryChampion(const char* name);
    Champion& tryChampion(std::string&& name);

    void readDataFile();
    void finalizeChampionsInitialization();

    private:
    using ChampionsMap = std::map<std::string, Champion>;
    ChampionsMap champions; ///< Map containing all Champions and associating them with a string ID (= name).

    using StagesMap = std::map<std::string, Stage>;
    StagesMap maps;
};