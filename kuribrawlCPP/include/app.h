#pragma once

#include <memory>
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Game.h"
#include "Port.h"
#include "GameData.h"
#include "ControllersData.h"

#define NB_PORTS 4

class Port;

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

    Port* joysticks[16]; //Pointer validity : can be invalidated if a Port is deleted
    private:
    void initSDL();
    void initControllersData();
    void handleEvents();
    void handleButtonEvent(const SDL_JoyButtonEvent* evt);
    void readPorts();
    void render();

    void startTestGame();

    //Data singletons
    std::unique_ptr<GameData> game_data;
    std::unique_ptr<ControllersData> controllers_data;

    SDL_Window* window;
    SDL_Renderer* renderer;

    Game* current_game; //Pointer validity : is invalitated when the game is destroyed (everytime a game ends)
    std::vector<Port> ports;
};