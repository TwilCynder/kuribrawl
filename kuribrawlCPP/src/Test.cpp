#include "app.h"
#include "SDL2/SDL_image.h"
#include "Debug.h"

void App::startTestGame(){

    Champion* acid = this->gameData.addChampion("acid");

	Animation* idle = acid->addAnimation("idle", IMG_LoadTexture(this->renderer, "../src/res/idle_strip4.png"));
	idle->initFrames(4);

	current_game = new Game();

	current_game->addFighter(acid, 100, 50);
	current_game->addFighter(acid, 500, 50);
}