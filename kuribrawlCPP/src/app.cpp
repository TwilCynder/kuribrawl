#include "sdlHelper.h"
#include "app.h"
#include "defs.h"

App::App(){

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
}

void App::init(){
	initSDL();
	this->testAnim = new Animation(IMG_LoadTexture(this->renderer, "../src/res/air_idle.png"));
}

void App::render(){
	SDLHelper::prepareRender(this->renderer);
	this->testAnim->draw(this->renderer, 50, 50);
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

			default:
				break;
		}
	}
}

void App::loop(){
    while(1){
        this->handleEvents();
		this->render();
        SDL_Delay(15);
    }
}