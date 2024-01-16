#include <vector>
#include "SDL2/SDL.h"
#include "ControllerType.h"

#define JOSTICKID_KEYBOARD -1
#define NB_CONTROLLERS 16

class Port;

class Controller {
    public:
    enum class SDLMode {
        JOYSTICK,
        GAMECONTROLLER,
        //DISCONNECTED,
        INVALID
    };

    using ButtonsMapping = std::vector<SDL_GameControllerButton>;

    static unique_ptr<Controller> opencontroller(int controller_id, ControllersData& cd);
    static unique_ptr<Controller> openKeyboardController(ControllersData& cd);

    void setControllerType(const ControllerType* c);

    private:
    Controller(int joystick_id);

    //These attributes determine the behavior of the controller or keyboard. They must be set (or unset, as long as everything is valid) everytime the controller changes
    //bool active; ///< True if the controller is active ; if false, the other attributes must not be used.
    const ControllerType* controller_type;
    SDL_GameController* gamecontroller; ///< MUST BE NULL IF NOT OPEN CURRENTLY
	SDL_Joystick* joystick;
    int joystick_id; ///< Numerical ID of the controller given by SDL, or -1 if using the keyboard.
    const ControllerLayout* current_controller_layout;    ///< The controller layout of the current controller, if any.
    //ButtonsMapping controller_buttons_mapping;

    Port* port; //port this controller is plugged in. Null if unpluged controlled.  

    bool isButtonPressed(int id) const;

    bool init(int controller_id, ControllersData& cd);
    bool initAsKeyboard(ControllersData& cd);
    bool initButtonMapping();

};