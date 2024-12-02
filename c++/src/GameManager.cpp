#include "GameManager.h"
#include "Game.h"
#include "KBDebug/Debug.h"

GameManager::GameManager() : 
    std::unique_ptr<Game>(nullptr)
{
}

void GameManager::destroy(){
    Game* ptr = release();
    if (ptr){
        delete ptr;
    }
}

Game& GameManager::start(){
    reset(new Game());
    return *get();
}

Game& GameManager::start(GameConfiguration& config){
    reset(new Game(config));
    return *get();
} 

Game& GameManager::start(GameConfSPtr& config){
    reset (new Game(config));
    Debug::log("Reset done");
    return *get();
}

void GameManager::stop(){
    destroy();
}

void GameManager::restart(){
    GameConfSPtr config_sptr;
    if (*this){
        config_sptr = get()->getOriginalConfigurationSharedPtr();
    } else {
        config_sptr = nullptr;
    }
    destroy();
    Debug::log("About to start new game ; previous game destroyed");
    start(config_sptr);
}