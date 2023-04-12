#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>

class Logger;

namespace SDLHelper {
    void prepareRender(SDL_Renderer* target);
    void render(SDL_Renderer* target);
    void initSDL();
    void closeSDL();

    //Debugging
    void printJoysticks(Logger& out);
}

