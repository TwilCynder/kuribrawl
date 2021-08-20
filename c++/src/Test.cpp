#include "app.h"
#include "SDL2/SDL_image.h"
#include "Debug.h"
#include "ControllerType.h"
#include "Fighter.h"
#include "Game.h"
#include "Port.h"
#include "ControllersData.h"

extern Parameters global_parameters;

void App::startTestGame(){
	current_game = new Game(global_parameters.slowness);

	Champion* acid = game_data->getChampion("acid");

	ports[0].setJoystick(0, this->controllersData().getController("PS4"));
	PlayerFighter* f1 = current_game->addFighter(acid, 100, 1, ports[0]);

	//ports[0].setFighter(f1);
}