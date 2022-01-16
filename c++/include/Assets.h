#pragma once
#include <string>
#include "Util/Dictionary.h"
#include "SDL2/SDL.h"

class Assets {
    public:
    Dictionnary<SDL_Texture*> textures;
};