#pragma once

#include "SDL2/SDL.h"

class App
{
    public:
    App();

    void init();
    void loop();

    SDL_Window* window;
    SDL_Renderer* renderer;

    private:
    void handleEvents();
    void render();
};