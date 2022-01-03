#include "app.h"
#include "SDL2/SDL_image.h"
#include "KBDebug/Debug.h"
#include "ControllerType.h"
#include "Fighter.h"
#include "Port.h"
#include "ControllersData.h"
#include "GameConfiguration.h"

void App::startTestGame(){

	Debug::log("=====Starting game=====");

	ports[0].plugController(-1, this->controllersData());
	
	Champion* acid = game_data->getChampion("acid");

	GameConfiguration config;
	config.addPlayer(&ports[0], *acid);
	config.addPlayer(nullptr, *acid);
	config.addPlayer(nullptr, *acid);

	Game& game = game_manager.start(config);

	Debug::log(game_manager.get());
}