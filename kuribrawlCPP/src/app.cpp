#include <iostream>

#include "Debug.h"
#include "sdlHelper.h"
#include "app.h"
#include "defs.h"
#include "Champion.h"
#include "GameData.h"

using namespace std;

App::App() : 
	ports{{Port(), Port(), Port(), Port()}}
{
	current_game = NULL;
}

App::~App(){

}

void App::initSDL(){
	int rendererFlags, windowFlags;

	if (!SDL_WasInit(0)){
		SDLHelper::initSDL();
	}

	rendererFlags = SDL_RENDERER_ACCELERATED;
	windowFlags = 0;

	this->window = SDL_CreateWindow("KuriX", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);
	
	if (!this->window){
		printf("Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
		exit(1);
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	this->renderer = SDL_CreateRenderer(this->window, -1, rendererFlags);
	
	if (!this->renderer){
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_JoystickEventState(SDL_ENABLE);
}

void App::init(){
	initSDL();
	Controller::initControllersData();
	startTestGame();
}

void App::render(){
	SDLHelper::prepareRender(this->renderer);
	if (current_game && current_game->is_running()){
		current_game->draw(renderer);
	}
	SDLHelper::render(this->renderer);
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
			case SDL_JOYBUTTONDOWN:
				
				{
					Port* port = Port::joysticks[event.jbutton.which];
					if (port != nullptr && port->isActive()){
						Debug::log(port->getInputBinding()->buttons[event.jbutton.button]);
					}
				}
				break;
			default:
				break;
		}
	}
}

void App::loop(){
    while(1){
        this->handleEvents();
		if (current_game && current_game->is_running()){
			current_game->advanceAnimations();
		}
		this->render();
        SDL_Delay(15);
    }
}