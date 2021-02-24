#include "SDL2/SDL.h"

class App
{
    public:
    App();

    void init();
    void loop();
    void handleEvents();

    SDL_Window* window;
    SDL_Renderer* renderer;
};