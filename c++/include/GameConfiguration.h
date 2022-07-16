#pragma once

#include <vector>
#include "PlayerConfiguration.h"

struct GameConfiguration{
    std::vector<PlayerConfiguration> players;
    //Add rules

    GameConfiguration();
    GameConfiguration(GameConfiguration&) = default;
    void addPlayer(PortRef, Champion&);
};