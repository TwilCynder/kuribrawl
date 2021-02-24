#include "sdlHelper.h"
#include "app.h"

App::App(){

}

void App::init(){
	SDLHelper::initSDL(this);
}

void App::render(){
	SDLHelper::prepareRender(this);
	SDLHelper::render(this);
}

void App::handleEvents(){
	SDLHelper::handleInputs(this);
}

void App::loop(){
    while(1){
        this->handleEvents();
		this->render();
        SDL_Delay(15);
    }
}