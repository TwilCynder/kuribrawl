#pragma once

#include <memory>
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Game.h"
#include "Port.h"
#include "GameData.h"
#include "ControllersData.h"

#define NB_PORTS 4

class App
{
    public:

    App();
    ~App();

    void init();
    void loop();
    void stop();

    SDL_Texture* loadTextureFromFile(const char* path);

    GameData& gameData();
    ControllersData& controllersData();

    private:
    void initSDL();
    void initControllersData();
    void handleEvents();
    void handleButtonEvent(const SDL_JoyButtonEvent* evt);
    void render();

    void startTestGame();

    //Data singletons
    std::unique_ptr<GameData> game_data;
    std::unique_ptr<ControllersData> controllers_data;

    SDL_Window* window;
    SDL_Renderer* renderer;

    Game* current_game;
    std::array<Port, NB_PORTS> ports;
};