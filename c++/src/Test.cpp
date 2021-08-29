#include "app.h"
#include "SDL2/SDL_image.h"
#include "Debug.h"
#include "ControllerType.h"
#include "Fighter.h"
#include "Game.h"
#include "Port.h"
#include "ControllersData.h"

void App::startTestGame(){
	current_game = new Game();

	Champion* acid = game_data->getChampion("acid");

	ports[0].setJoystick(0, this->controllersData());
	PlayerFighter* f1 = current_game->addFighter(acid, 100, 1, ports[0]);

	//ports[0].setFighter(f1);
}