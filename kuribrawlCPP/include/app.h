#pragma once

#include <forward_list>
#include <array>
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Game.h"
#include "Port.h"
#include "GameData.h"
#include "input.h"

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

    GameData gameData;

    private:
    void initSDL();
    void handleEvents();
    void render();

    void startTestGame();

    SDL_Window* window;
    SDL_Renderer* renderer;

    Game* current_game;
    std::array<Port, NB_PORTS> ports;
};