#pragma once

#include <forward_list>
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Game.h"
#include "GameData.h"
#include "input.h"

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
    SDL_Window* window;
    SDL_Renderer* renderer;
    void initSDL();
    void handleEvents();
    void render();

    void startTestGame();

    Game* current_game;
    std::forward_list<RegisteredInput> inputQ;
};