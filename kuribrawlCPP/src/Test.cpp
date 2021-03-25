#include "app.h"
#include "SDL2/SDL_image.h"
#include "Debug.h"
#include "ControllerType.h"
#include "Fighter.h"

void App::startTestGame(){
    Champion* acid = this->gameData().addChampion("acid");
	Animation* idle = acid->addAnimation("idle", IMG_LoadTexture(this->renderer, "../src/res/idle_strip4.png"));

	idle->initFrames(4);

	current_game = new Game();

	Fighter* f1 = current_game->addFighter(acid, 100, 50);
	//current_game->addFighter(acid, 500, 50);

	ports[0].setJoystick(0, this->controllersData().getController("GC"));
	ports[0].setFighter(f1);
}