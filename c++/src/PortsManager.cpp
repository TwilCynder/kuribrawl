#include "PortsManager.h"

#define PORTS_NB 4

PortsManager::PortsManager(){
    ports.reserve(PORTS_NB);
    for (int i = 0; i< PORTS_NB; i++){
        ports.emplace_back(this, i);
    } 

    for (int i = 0; i < NB_CONTROLLERS; i++){
		controllers[i] = nullptr;
	}
	keyboard = nullptr;
}

void PortsManager::initSDL()
{
	keyboard_state = SDL_GetKeyboardState(nullptr);
}

void PortsManager::readPorts(){
	for (unsigned int i = 0; i < ports.size(); i++){
		if (ports[i].isActive()){
			ports[i].readController();
		}
	}
}

/**
 * @brief Called when a controller button is pressed : calls the corresponding Port's handling method.
 *
 * @param evt The event structure containing all the info on the button press.
 */

void PortsManager::handleButtonEvent(const SDL_JoyButtonEvent& evt){
	Port* port = controllers[evt.which];
	Debug::log(evt.button);
	if (port != nullptr && port->isActive()){
		port->handleJoystickButtonPress(evt.button);
	}
}

void PortsManager::handleKeyEvent(const SDL_KeyboardEvent &evt){
    if (keyboard && !evt.repeat)
        keyboard->handleButtonPress(evt.keysym.scancode);
}
