#include "sdlHelper.h"
#include "KBException.h"
#include "KBDebug/Debug.h"

namespace SDLHelper {

void initSDL()
{

	int res = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

	if ( res < 0){
		printf("Couldn't initialize SDL (%d): %s\n", res, SDL_GetError());
		exit(1);
	}

	res = IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	if (res < 0) {
		throw new KBFatal(Kuribrawl::formatString("Couldn't initialize SDL Image (%d): %s\n", res, SDL_GetError()));
	};
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

void printJoysticks(Logger& out){
	SDL_Joystick* joy;
	Debug::log("-------- Available joysticks ---------");
	for (int i = 0; i < SDL_NumJoysticks(); i++){
		joy = SDL_JoystickOpen(i);
		out << i << " : " << SDL_JoystickName(joy) << '\n';
		SDL_JoystickClose(joy);
	}

	Debug::log("------------------------");
}


}