#include "PortsManager.h"
#include "Controllers.h"

#define PORTS_NB 4

PortsManager::PortsManager(){
    ports.reserve(PORTS_NB);
    for (int i = 0; i< PORTS_NB; i++){
        ports.emplace_back(*this, i);
    } 

    for (int i = 0; i < NB_CONTROLLERS; i++){
		controllers[i] = nullptr;
	}
	keyboard_controller = nullptr;
}

PortsManager::~PortsManager() = default;

void PortsManager::openController(int controller_id, ControllersData& cd){

	auto controller_ptr = Controller::opencontroller(controller_id, cd);
	int id = controller_ptr->getJoystickID();

	ControllerPtr& current_controller = controllers[id];
	//TODO remove that controller

	controllers[id] = std::move(controller_ptr);
}

void PortsManager::openKeyboardController(ControllersData &cd)
{
	keyboard_controller = std::move(Controller::openKeyboardController(cd));
}

void PortsManager::plugController(uint8_t portID, Controller& controller){
	if (portID >= PORTS_NB){
		throw KBFatal("Attemp to plug controller in port %d, which is above the maximum (%d)", portID, PORTS_NB);
	}

	ports[portID].plugController(controller);
}

void PortsManager::plugKeyboard(uint8_t portID)
{
	if (keyboard_controller){
		plugController(portID, *keyboard_controller);
	}
}

void PortsManager::plugController(uint8_t portID, uint8_t controllerID)
{
    if (controllerID > NB_CONTROLLERS){
		throw KBFatal("Attemp to access controller %d, which is above the maximum (%d)", controllerID, NB_CONTROLLERS);
	}

	if (controllers[controllerID]){
		plugController(portID, *controllers[controllerID]);
	}
}

Port* PortsManager::getPort(uint8_t id) {
	if (id >= PORTS_NB){
		throw KBFatal("Attemp to access port %d, which is above the maximum (%d)", id, PORTS_NB);
	}	
	return &ports[id];
}

void PortsManager::removeController(int id)
{
	if (id > NB_CONTROLLERS){
		throw KBFatal("Attemp to access controller %d, which is above the maximum (%d)", id, NB_CONTROLLERS);
	}

	Controller* cont = controllers[id].get();

	if (cont){
		cont->unplugFromPort();
		controllers[id].reset();
	} else {
		Debug::warn(formatString("Tried to remove controller %d, which was not open"));
	}
}

void PortsManager::initSDL()
{

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
	Controller* controller = controllers[evt.which].get();
	Debug::log(evt.button);
	if (controller != nullptr){
		controller->handleJoystickButtonPress(evt.button);
	}

}

void PortsManager::handleKeyEvent(const SDL_KeyboardEvent &evt){
	Controller* controller = keyboard_controller.get();
    if (controller != nullptr && !evt.repeat)
        controller->handleButtonPress(evt.keysym.scancode);
}