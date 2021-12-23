#include "GameManager.h"
#include "Game.h"

GameManager::GameManager() : 
    std::unique_ptr<Game>(nullptr)
{
}

/*
Game* GameManager::getGame(){
    return game.get();
}
*/

void GameManager::start(){
    *this = std::make_unique<Game>();
}