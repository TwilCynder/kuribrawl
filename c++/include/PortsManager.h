#pragma once

#include "Port.h"

#define NB_CONTROLLERS 16

class Controller;

class PortsManager {
    std::vector<Port> ports; ///< Currently active Ports.

    public:
    PortsManager();
    ~PortsManager();

    void initSDL();

    Controller& openController(int controller_id, ControllersData& cd);
    Controller& openKeyboardController(ControllersData& cd);
    void openAllControllers(ControllersData& cd);
    void plugController(uint8_t portID, Controller& controller);
    //these ones are mostly for debug
    void plugKeyboard(uint8_t portID);
    void plugController(uint8_t portID, uint8_t controller_id);

    void removeController(int id);

    void readPorts();

    void handleButtonEvent(const SDL_JoyButtonEvent& evt);
    void handleKeyEvent(const SDL_KeyboardEvent & evt);


    //Port* controllers[NB_CONTROLLERS];    ///<An array associating each joystick numerical id with a Port. 
    //                        /**<Pointer validity : can be invalidated if a Port is deleted or moved */
    //Port* keyboard; ///<Port associated with the keyboard


    Port* getPort(uint8_t id);
    Port* getFirstInactivePort();

    using ControllerPtr = std::unique_ptr<Controller>;
    ControllerPtr keyboard_controller;
    ControllerPtr controllers[NB_CONTROLLERS];
};