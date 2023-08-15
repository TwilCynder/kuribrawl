#include "Util/Drawing.h"
#include "SDL2/SDL.h"

void SDL_Drawing::drawCross(SDL_Renderer* target, int x, int y, int size){
    SDL_RenderDrawLine(target, x - size, y, x + size, y);
    SDL_RenderDrawLine(target, x, y - size, x, y + size);
}