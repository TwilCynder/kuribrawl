#include <iostream>

#include "Debug.h"
#include "sdlHelper.h"
#include "app.h"
#include "defs.h"
#include "Champion.h"
#include "GameData.h"
#include "InputManager.h"
#include "Port.h"

using namespace std;

App::App() : 
	ports{Port(this), Port(this), Port(this), Port(this)},
	game_data(std::make_unique<GameData>()),
	controllers_data(std::make_unique<ControllersData>())
{
	current_game = NULL;
}

App::~App(){

}

GameData& App::gameData(){
	return *game_data;
}

ControllersData& App::controllersData(){
	return *controllers_data;
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

void App::initControllersData(){
	controllers_data->initControllersData();
}

void App::init(){
	initSDL();
	InputManager::initInputHandlers();
	initControllersData();
	startTestGame();
}

void App::render(){
	SDLHelper::prepareRender(this->renderer);
	if (current_game && current_game->is_running()){
	}
	SDLHelper::render(this->renderer);
}

void App::handleButtonEvent(const SDL_JoyButtonEvent* evt){
	Port* port = joysticks[evt->which];
	if (port != nullptr && port->isActive()){
		port->handleButtonPress(evt->button);
	}
}

void App::readPorts(){
	for (int i = 0; i < ports.size(); i++){
		if (ports[i].isActive()){
			ports[i].readController();
		}
	}
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
				handleButtonEvent(&event.jbutton);
				break;
			default:
				break;
		}
	}
}

void App::loop(){
    while(1){
        this->handleEvents();
		SDLHelper::prepareRender(this->renderer);
		readPorts();
		if (current_game && current_game->is_running()){
			current_game->updateStates();
			current_game->updateInputs();
			current_game->applyPhysics();
			current_game->draw(renderer);
			current_game->advanceAnimations();
		}
		SDLHelper::render(this->renderer);
        SDL_Delay(15);
    }
}