#pragma once

#include "Port.h"

#define NB_CONTROLLERS 16

class Controller;

class PortsManager {
    std::vector<Port> ports; ///< Currently active Ports.

    public:
    PortsManager();

    void initSDL();

    void plugController(uint8_t portID, int8_t controller, ControllersData&);
    void readPorts();

    void handleButtonEvent(const SDL_JoyButtonEvent& evt);
    void handleKeyEvent(const SDL_KeyboardEvent & evt);

    Port* controllers[NB_CONTROLLERS];    ///<An array associating each joystick numerical id with a Port. 
                            /**<Pointer validity : can be invalidated if a Port is deleted or moved */
    Port* keyboard; ///<Port associated with the keyboard
    
    const Uint8* keyboard_state;

    Port* getPort(uint8_t id);
    Port* getFirstInactivePort();

    void openController(int controller_id, ControllersData& cd);

    using ControllerPtr = std::unique_ptr<Controller>;
    ControllerPtr keyboard_controller;
    ControllerPtr controllers[NB_CONTROLLERS];

    void removeController(int id);
};