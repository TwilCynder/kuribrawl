#pragma once

#include <vector>
#include "PlayerConfiguration.h"

class StageModel;

struct GameConfiguration{
    std::vector<PlayerConfiguration> players;
    const StageModel* stage;
    //Add rules

    GameConfiguration();
    GameConfiguration(GameConfiguration&) = default;
    
    void addPlayer(PortRef, Champion&);
    void setStage(const StageModel*);
};