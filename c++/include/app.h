#pragma once

#include <memory>
#include <vector>
#include <fstream>
#include "SDL2/SDL.h"
#include "SDL2/SDL_syswm.h"
#include "Display/Animation.h"
#include "GameData.h"
#include "Assets.h"
#include "GameManager.h"
#include "PortsManager.h"
#include "Util/Text/TextureFont.h"
#include "Util/Text/TextDisplayer.h"
#include "Util/StaticFullQueue.h"
#include "KBException.h"

class Port;
class Game;
class ControllersData;


/**
 * @brief A number of miliseconds elapsed since SDL initialization
 */
typedef long double Date;

typedef long double Duration;

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
    void close();
    void loop();
    void stop();

    GameData& gameData();
    ControllersData& controllersData();
    Assets& assets();

    void setFrameRate(int fr);
    void viewPopupMessage(const char* title, const char* message);

    //SDL functions
    SDL_Texture* LoadTexture(const char *file);

    PortsManager portsManager;

    private:
    void initSDL();
    void initControllersData();
    void initGameData();
    void loadRessources();
    bool loadGameFile(const char* name);

    void handleEvents();
    void handleButtonEvent(const SDL_JoyButtonEvent& evt);
    void handleDeviceAdded(const SDL_JoyDeviceEvent& evt);
    void handleDeviceRemoved(const SDL_JoyDeviceEvent& evt);
    void render();

    void startTestGame();

    void update_frame_duration();
    void loop_timer();
    bool initialized;
    Date next_frame_date; ///< Date at which the next frame is supposed to start
    Date start_time;
    int framerate; ///< Frames displayed per second
    double frame_duration; ///< Duration of a frame in miliseconds
    bool paused;
    bool advance;

    bool quit;

    void handleExcept(KBFatal& ex);

    //Data singletons
    std::unique_ptr<GameData> game_data; //*< the GameData (all the data related to gameplay) that will be used for games ran by this App.
    std::unique_ptr<ControllersData> controllers_data; //*< the ControllersData (all the data related to reading controllers inputs) that will be used by this App.
    Assets assets_; /** \todo tranformer en unique_pointer */

    SDL_Window* window; ///< The window this app is running in.
    SDL_SysWMinfo win_info; ///< Structure containing info about the main window.
    SDL_Renderer* renderer; ///< The SDL renderer used to render this app.

    GameManager game_manager;
    //Game* current_game; 

    //Performance test
    StaticFullQueue<Duration, 30> last_frames_wait;
    Duration getLowestWait();
    int frame; ///< Number of frames displayed since the loop was started.
    int total_frame_wait;
    void print_report(Logger&);

    //Debug
    std::shared_ptr<TextureFont> debugFont;
    std::unique_ptr<AnchoredTextDisplayer> debug_text_displayer;
    void drawText(const std::string s, int x, int y);
    void drawDebugInfo();
};