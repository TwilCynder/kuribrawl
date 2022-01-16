#include <iostream>
#include <ctime>
#include <limits>
#include "KBDebug/Debug.h"
#include "sdlHelper.h"
#include "app.h"
#include "defs.h"
#include "Champion.h"
#include "GameData.h"
#include "Game.h"
#include "InputManager.h"
#include "Port.h"
#include "hardCoded.h"
#include "ControllersData.h"
#include "DataFile.h"
#include "System.h"
#include "messageBox.h"
#include "Random.h"
#include "Text/TextDisplayer.h"

using namespace std;

/**
 * @brief Construct a new App object
 *
 */
App::App() :
	App(60)
{
}

App::App(int framerate):
	paused(false),
	advance(false),
	quit(false),
	game_data(std::make_unique<GameData>()),
	controllers_data(std::make_unique<ControllersData>()),
	ports{Port(this), Port(this), Port(this), Port(this)},
	last_frames_wait(std::numeric_limits<Duration>::max())
{
	setFrameRate(framerate);
}

App::~App(){
	close();
}

/**
 * @brief Returns the currently used game data.
 * Doesn't follow usual getter nomenclature for historical reasons.
 * @return GameData&
 */
GameData& App::gameData(){
	return *game_data;
}

/**
 * @brief Returns the currently used controllers data.
 * Doesn't follow usual getter nomenclature for historical reasons.
 * @return ControllersData&
 */
ControllersData& App::controllersData(){
	return *controllers_data;
}

Assets& App::assets() {
	return assets_;
}

/**
 * @brief Returns the first \ref Port#isActive "inactive" port or nullptr if there is none.
 * 
 * @return Port* 
 */
Port* App::getFirstInactivePort(){
	for (unsigned int i = 0; i < ports.size(); i++){
		if (!ports[i].isActive()){
			return &ports[i];
		}
	}
	return nullptr;
}

/**
 * @brief Initializes SDL components.
 * Initializes :
 * - SDL Core
 * - SDL Joystick
 */
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

	SDL_VERSION(&win_info.version);
	SDL_GetWindowWMInfo(window, &win_info);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	this->renderer = SDL_CreateRenderer(this->window, -1, rendererFlags);

	if (!this->renderer){
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_JoystickEventState(SDL_ENABLE);
	keyboard_state = SDL_GetKeyboardState(nullptr);
}

/**
 * @brief Loads a texture using the current renderer.
 * Texture are surfaces managed directly by tht GPU, which is why a renderer needs to be specified when creating/loading them.
 * @param file
 * @return SDL_Texture*
 */

SDL_Texture* App::LoadTexture(const char *file){
	return IMG_LoadTexture(renderer, file);
}

/**
 * @brief Initializes the game data with all hardcoded data.
 *
 */

void App::initGameData(){
	HardCoded::initGameData(this);
	game_data->finalizeChampionsInitialization();
}

bool App::loadGameFile(const char* name){
	cout << "Using data file at " << name << '\n' << std::flush;

    DataFile data_file(name, renderer);
    if (data_file.ready()) {
        data_file.read(*this);
        return true;
    }
    return false;

}

/**
 * @brief Loads game data from a data file (data.twl).
 * Tries different locations for the data.twl file.
 */
void App::loadRessources(){
	if (!loadGameFile("data.twl"))
	if (!loadGameFile("../res/data.twl"))
	if (!loadGameFile("../gamefile manager/data.twl"))
	throw KBFatalExplicit("Can't find data file");
}

/**
 * @brief Initializes controllers data with all hardcoded data.
 *
 */

void App::initControllersData(){
	HardCoded::initControllersData(*controllers_data);
}

/**
 * @brief Does everything that needs to be done before the loop starts (including of course initialization).
 * Calls all the initialization functions above (SDL, game data, controllers data) ;
 * also starts the test game.
 */

void App::init(){
	initSDL();
	SDLHelper::printJoysticks(cout);
	PlayerFighter::initInputHandlers();
	initControllersData();
	loadRessources();
	initGameData();
	startTestGame();
	initialized = true;

	SDL_Texture* debug_font = assets_.textures.get("font_oracle");
	if (!debug_font) throw KBFatalDetailed("Missing font", "Cannot find the debug font");
	debugFont = make_unique<TextureFont>(TextureFont(debug_font, renderer, {8, 11}));
	debug_text_displayer = make_unique<AnchoredTextDisplayer>(SCREEN_WIDTH - 88, 20, *debugFont);
}

void App::close(){
	if (initialized){
		SDLHelper::closeSDL();
	}
}

/**
 * @brief Renders the app, i.e. renders the current game or menu if there is any.
 *
 */

void App::render(){
	SDLHelper::prepareRender(this->renderer);
	if (game_manager.get() && game_manager->is_running()){
	}
	SDLHelper::render(this->renderer);
}

/**
 * @brief Called when a controller button is pressed : calls the corresponding Port's handling method.
 *
 * @param evt The event structure containing all the info on the button press.
 */

void App::handleButtonEvent(const SDL_JoyButtonEvent* evt){
	Port* port = controllers[evt->which];
	if (port != nullptr && port->isActive()){
		port->handleButtonPress(evt->button);
	}
}

/**
 * @brief Handle input reading for all Ports
 * Calls the input reading method of all active Ports.
 */

void App::readPorts(){
	for (unsigned int i = 0; i < ports.size(); i++){
		if (ports[i].isActive()){
			ports[i].readController();
		}
	}
}

void App::print_report(std::ostream& out){
	Uint32 app_duration = System::now() - start_time;
	out << "Time elapsed : " << app_duration << "ms. Frames displayed : " << frame << ". Mean frame duration : " << (double)app_duration / frame << ". Mean frame wait " << ((double)total_frame_wait / frame) << '\n';
}

Duration App::getLowestWait(){
	Duration min = std::numeric_limits<Duration>::max();
	for (Duration d : last_frames_wait){
		if (d < min) min = d;
	}

	return min;
}

void App::drawDebugInfo(){
	debug_text_displayer->reset();
	*debug_text_displayer << getLowestWait();
}

/**
 * @brief Stops the program.
 *
 * @param code the exit code.
 */
void App::stop(){
	print_report(cout);
	quit = true;
}

/**
 * @brief Handle SDL events.
 * Currently handled events :
 * - Close window
 * - Joystick button pressed.
 */

void App::handleEvents(){
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				stop();
				break;
			case SDL_CONTROLLERBUTTONDOWN:
				handleButtonEvent(&event.jbutton);
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode){
					case SDL_SCANCODE_ESCAPE:
						stop();
						break;
					case SDL_SCANCODE_RETURN:
						paused = !paused;
						break;
					case SDL_SCANCODE_SPACE:
						advance = true;
						break;
					case SDL_SCANCODE_F5:
						game_manager.restart();
						break;
					case SDL_SCANCODE_F6:
						setFrameRate(framerate - 5);
						break;
					case SDL_SCANCODE_F7:
						setFrameRate(framerate + 5);
						break;
					default:
						if (!event.key.repeat)
							keyboard->handleButtonPress(event.key.keysym.scancode);
						break;
				}
				break;
			default:
				break;
		}
	}
}

/**
 * @brief Sets the framerate at will the app loop will run.
 * @param fr
 */
void App::setFrameRate(int fr){
	if (fr < 0 && fr != -1){
		throw KBFatal("Attemp to use invalid framerate");
	}
	framerate = fr;
	update_frame_duration();
}

/**
 * @brief On OSes that support it, show an OS-native popup message.
 * App::InitSDL must have been successfully called before calling this method.
 */
void App::viewPopupMessage(LPCSTR title, LPCSTR message){
	Kuribrawl::errorPopup(win_info.info.win.window, title, message);
}

/**
 * @brief Update the duration of a a frame based on the framerate.
 */
void App::update_frame_duration(){
	frame_duration = 1000.0 / framerate;
}

/**
 * @brief Manages all operations related to making the loop run at the desired speed (framerate), at the end of each iteration.
 */
void App::loop_timer(){
	Date current_date = System::now();
	Duration wait = next_frame_date - current_date;
	if (wait < 0){
		next_frame_date = current_date;
	} else {
		SDL_Delay(wait);
	}

	last_frames_wait.add(wait);

	total_frame_wait += wait;
}

/**
 * @brief Main loop of the game.
 * Never returns ; the loop is infinite and stops only when exit() is called.
 */

void App::loop() try {
	frame = 0;
	total_frame_wait = 0;
	next_frame_date = System::now();
	start_time = next_frame_date;

    while(!quit){
		next_frame_date += frame_duration;
        handleEvents();

		if (!paused || advance){
			advance = false;
			readPorts();
			SDLHelper::prepareRender(renderer);
			if (game_manager.get() && game_manager->is_running()){
				game_manager->step(renderer);
				game_manager->drawDebugInfo(*debugFont);
			}
			drawDebugInfo();
			SDLHelper::render(renderer);
			frame++;
		}

        loop_timer();
    }
} catch (KBFatal& exception){
	exception.setData(win_info.info.win.window);
	throw;
}