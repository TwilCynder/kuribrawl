#include "defs.h"
#include "app.h"

App::App(){

}

void App::init(){
	
}

void App::handleEvents(){
    SDL_Event event;
	
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				exit(0);
				break;

			default:
				break;
		}
	}
}

void App::loop(){

}