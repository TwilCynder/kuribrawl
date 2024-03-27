#pragma once

#include <vector>
#include <memory>
#include "SDL2/SDL.h"
#include "ControllerType.h"
#include "ControllerVals.h"
#include "controllerElements.h"
#include "inputs.h"
#include "Port.h"

#define JOYSTICKID_KEYBOARD -1
#define NB_CONTROLLERS 16

class ControllersData;

class Controller {
    public:
    Controller();

    enum class SDLMode {
        JOYSTICK,
        GAMECONTROLLER,
        //DISCONNECTED,
        INVALID
    };

    using ButtonsMapping = std::vector<SDL_GameControllerButton>;

    static std::unique_ptr<Controller> opencontroller(int controller_id, ControllersData& cd);
    static std::unique_ptr<Controller> openKeyboardController(ControllersData& cd); 

    void plugToPort(Port&); //must be called ONLY by Port::plugController
    void unplugFromPort();
    void clearPort(); //must be called ONLY by Port::unplugController
    void closeController();

    const ControllerType* getControllerType() const;
    void setControllerType(const ControllerType* c);

    int getJoystickID() const;

    bool isButtonPressed(int id) const;
    bool isJoystickButtonPressed(int button) const;
    bool isTriggerPressed(int trigger) const;
    bool isTriggerPressed(int trigger, const ControllerVals& vals) const;
    bool isElementPressed(ElementType type, int element) const;
    bool isElementPressed(ElementType type, int element, const ControllerVals& vals) const;

    void readController(Port::ElementsState& state) const;

    void handleButtonPress(Uint8);
    void handleJoystickButtonPress(Uint8);

    signed char getDpadStateX() const;
    signed char getDpadStateY() const;


    private:
    Controller(int joystick_id);

    //These attributes determine the behavior of the controller or keyboard. They must be set (or unset, as long as everything is valid) everytime the controller changes
    //bool active; ///< True if the controller is active ; if false, the other attributes must not be used.
    const ControllerType* controller_type;
    SDL_GameController* gamecontroller;
	SDL_Joystick* joystick;
    int joystick_id; ///< Numerical ID of the controller given by SDL, or -1 if using the keyboard.
    int device_id; ///< Numerical ID of the device (given by the OS)
    const ControllerLayout* current_controller_layout;    ///< The controller layout of the current controller, if any.
    ButtonsMapping controller_buttons_mapping;

    Port* port; //port this controller is plugged in. Null if unpluged controlled.  

    bool init(int controller_id, ControllersData& cd);
    bool initAsKeyboard(ControllersData& cd);
    bool initButtonMapping();


};