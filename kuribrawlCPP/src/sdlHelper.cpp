#include "sdlHelper.h"

namespace SDLHelper {

void initSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	IMG_Init(IMG_INIT_PNG);
}

void prepareRender(SDL_Renderer* target)
{
    SDL_SetRenderDrawColor(target, 255, 0, 0, 255);
	SDL_RenderClear(target);
}

void render(SDL_Renderer* target)
{
	SDL_RenderPresent(target);
}


}