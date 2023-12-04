#include "ControllerType.h"

#define NB_CONTROLLERS 16

class Controller {
    public:
    enum class SDLMode {
        JOYSTICK,
        GAMECONTROLLER,
        //DISCONNECTED,
        INVALID
    };

    //These attributes determine the behavior of the controller or keyboard. They must be set (or unset, as long as everything is valid) everytime the controller changes
    bool active; ///< True if the controller is active ; if false, the other attributes must not be used.
    const ControllerType* controller_type;
    SDL_GameController* controller; ///< MUST BE NULL IF NOT OPEN CURRENTLY
	SDL_Joystick* joystick;
    int joystick_id; ///< Numerical ID of the controller given by SDL, or -1 if using the keyboard.

    Port* port; //port this controller is plugged in. Null if unpluged controlled.  

    virtual bool isButtonPressed(int id) const;
    
    

};

class ControllersManager {
    std::unique_ptr<Controller> keyboard_controller;
    std::unique_ptr<Controller> controllers[NB_CONTROLLERS 16];

    void removeController(int id);
}