#pragma once

#include "SDL2/SDL.h"
#include "Animation.h"

class App
{
    public:
    App();
    ~App();

    void init();
    void loop();
    void stop();

    SDL_Texture* loadTextureFromFile(const char* path);

    private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    void initSDL();
    void handleEvents();
    void render();

    //test
    Animation* testAnim;
};