#include "sdlHelper.h"

namespace SDLHelper {

void initSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0){
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
}

void closeSDL(){
	SDL_Quit();
	IMG_Quit();
}

void prepareRender(SDL_Renderer* target)
{
    SDL_SetRenderDrawColor(target, 0, 0, 0, 255);
	SDL_RenderClear(target);
}

void render(SDL_Renderer* target)
{
	SDL_RenderPresent(target);
}

void printJoysticks(std::ostream& out){
	SDL_Joystick* joy;
	out << "---- Available joysticks -----" << '\n';
	for (int i = 0; i < SDL_NumJoysticks(); i++){
		joy = SDL_JoystickOpen(i);
		out << i << " : " << SDL_JoystickName(joy) << '\n';
		SDL_JoystickClose(joy);
	}

	out << "---------" << '\n'  << std::flush;
}


}