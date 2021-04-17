#include "System.h"
#include "SDL2/SDL_timer.h"

Uint32 System::now(){
    return SDL_GetTicks();
}