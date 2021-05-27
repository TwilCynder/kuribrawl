#pragma once

#include <memory>
#include <vector>
#include <fstream>
#include "SDL2/SDL.h"
#include "SDL2/SDL_syswm.h"
#include "Animation.h"
#include "GameData.h"

#define NB_PORTS 4

class Port;
class Game;
class ControllersData;

/**
 * @brief A number of miliseconds elapsed since SDL initialization
 */
typedef Uint32 Date;

/**
 * @brief An instance of the program.
 * Contains absolutely every persistent data needed by the program (static data like configurations, and current game data).
 */

class App
{
    public:

    App();
    App(int framerate);
    ~App();

    void init();
    void loop();
    void stop(int code);

    GameData& gameData();
    ControllersData& controllersData();

    void setFrameRate(int fr);
    void viewPopupMessage(const char* title, const char* message);

    //SDL functions
    SDL_Texture* LoadTexture(const char *file);

    Port* joysticks[16];    ///<An array associating each joystick numerical id with a Port. 
                            /**<Pointer validity : can be invalidated if a Port is deleted or moved */
    private:
    void initSDL();
    void initControllersData();
    void initGameData();
    void loadRessources();
    void handleEvents();
    void handleButtonEvent(const SDL_JoyButtonEvent* evt);
    void readPorts();
    void render();

    void startTestGame();

    void update_frame_duration();
    void loop_timer();
    Date next_frame_date; ///< Date at which the next frame is supposed to start
    Date start_time;
    int framerate; ///< Frames displayed per second
    double frame_duration; ///< Duration of a frame in miliseconds

    //Data singletons
    std::unique_ptr<GameData> game_data; //*< the GameData (all the data related to gameplay) that will be used for games ran by this App.
    std::unique_ptr<ControllersData> controllers_data; //*< the ControllersData (all the data related to reading controllers inputs) that will be used by this App.

    SDL_Window* window; ///< The window this app is running in.
    SDL_SysWMinfo win_info; ///< Structure containing info about the main window.
    SDL_Renderer* renderer; ///< The SDL renderer used to render this app.

    Game* current_game; ///< The Game that is currently running, if there is any. Pointer validity : is invalitated when the game is destroyed (everytime a game ends) .*/

    std::vector<Port> ports; ///< Currently active Ports.

    //Performance test
    int frame; ///< Number of frames displayed since the loop was started.
    void print_report(std::ostream& out);
};