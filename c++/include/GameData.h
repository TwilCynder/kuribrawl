#pragma once
#include <map>
#include <string>
#include "Champion.h"
#include "Stage.h"
#include "util/stringOperations.h"

class App;

/**
 * @brief Class containing all data related to gameplay.
 * This includes every Champion, Stage, etc
 */
class GameData {
    public:

    Champion* getChampion(const char* name);
    Champion* getChampion(const std::string& name);
    Champion& addChampion(const std::string_view& name);
    Champion& addChampion(std::string&& name);
    Champion& tryChampion(const Kuribrawl::string_view& name);
    Champion& tryChampion(std::string&& name);
    Champion& tryChampion(const std::string& name);

    Stage* getStage(const char* name);
    Stage* getStage(const std::string& name);
    Stage& addStage(const std::string_view& name);
    Stage& addStage(std::string&& name);
    Stage& tryStage(const Kuribrawl::string_view& name);
    Stage& tryStage(std::string&& name);
    Stage& tryStage(const std::string& name);

    void finalizeChampionsInitialization();

    private:
    using ChampionsMap = std::map<std::string, Champion, std::less<>>;
    ChampionsMap champions; ///< Map containing all Champions and associating them with a string ID (= name).

    using StagesMap = std::map<std::string, Stage, std::less<>>;
    StagesMap stages;
};