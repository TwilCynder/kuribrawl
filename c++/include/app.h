#pragma once

#include <memory>
#include <vector>
#include "SDL2/SDL.h"
#include "Animation.h"
#include "GameData.h"

#define NB_PORTS 4

class Port;
class Game;
class ControllersData;

class App
{
    public:

    App();
    ~App();

    void init();
    void loop();
    void stop();

    GameData& gameData();
    ControllersData& controllersData();

    //SDL functions
    SDL_Texture* LoadTexture(const char *file);

    Port* joysticks[16]; //Pointer validity : can be invalidated if a Port is deleted or moved
    private:
    void initSDL();
    void initControllersData();
    void initGameData();
    void loadRessources();
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