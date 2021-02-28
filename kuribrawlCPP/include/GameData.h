#pragma once
#include "Champion.h"
#include <map>
#include <string>

class GameData {
    public:

    Champion* getChampion(const std::string& name);
    Champion* addChampion(const std::string& name);

    private:
    std::map<std::string, Champion> champions;
};