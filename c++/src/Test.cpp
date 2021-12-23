#include "app.h"
#include "SDL2/SDL_image.h"
#include "KBDebug/Debug.h"
#include "ControllerType.h"
#include "Fighter.h"
#include "Game.h"
#include "Port.h"
#include "ControllersData.h"

void App::startTestGame(){

	Debug::log("===Starting game=====");

	current_game = new Game();

	Champion* acid = game_data->getChampion("acid");

	ports[0].plugController(-1, this->controllersData());
	current_game->addFighter(acid, 100, 1, ports[0]);
	current_game->addFighter(acid, 300, 1);
	current_game->addFighter(acid, 500, 1);

	//ports[0].setFighter(f1);
}