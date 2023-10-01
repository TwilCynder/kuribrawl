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
    
    void addPlayer(ConstPortRef, Champion&);
    void setStage(const StageModel*);
};