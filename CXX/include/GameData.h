#pragma once
#include <map>
#include <string>
#include "Champion.h"

class App;

class GameData {
    public:

    Champion* getChampion(const std::string& name);
    Champion* addChampion(const std::string& name);
    Champion& tryChampion(const std::string& name);

    void readDataFile();
    void finalizeChampionsInitialization();

    private:
    std::map<std::string, Champion> champions;
};