#pragma once

#include "Port.h"

#define NB_CONTROLLERS 16

class PortsManager {
    std::vector<Port> ports; ///< Currently active Ports.

    public:
    PortsManager();

    void initSDL();

    void readPorts();

    void handleButtonEvent(const SDL_JoyButtonEvent& evt);
    void handleKeyEvent(const SDL_KeyboardEvent & evt);

    Port* controllers[NB_CONTROLLERS];    ///<An array associating each joystick numerical id with a Port. 
                            /**<Pointer validity : can be invalidated if a Port is deleted or moved */
    Port* keyboard; ///<Port associated with the keyboard
    
    const Uint8* keyboard_state;

    Port* getFirstInactivePort();
};