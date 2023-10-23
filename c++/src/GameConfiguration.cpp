#include "GameConfiguration.h"

GameConfiguration::GameConfiguration()
{
}

void GameConfiguration::addPlayer(PortRef port, Champion& champion){
    players.emplace_back(port, champion);
}

void GameConfiguration::setStage(const StageModel* stage){
    this->stage = stage;
}