#pragma once

#include <forward_list>
#include <vector>
#include "PlayerFighter.h"
#include "ControllerType.h"
#include "inputs.h"
#include "Util/util.h"
#include "PortOptimizationData.h"

#define JOSTICKID_KEYBOARD -1

#define ANALOG_MAX_VALUE 32000

class App;
class Binding;
class ControllersData;
class PortsManager;

class Port {
    public:

    Port(PortsManager& pm_, int id);

    bool isActive() const;
    void plugController(int8_t id, ControllersData& cd);
    void setFighter(PlayerFighter*);
    void initOptimizationData();
    void deactivate();
    void setControllerType(const ControllerType* c);
    const ControllerType* getControllerType() const;

    void handleButtonPress(Uint8);
    void handleJoystickButtonPress(Uint8);
    bool isButtonPressed(int button) const;
    bool isJoystickButtonPressed(int button) const;
    bool isTriggerPressed(int trigger) const;
    bool isTriggerPressed(int trigger, const ControllerVals& vals) const;
    bool isElementPressed(ElementType type, int element) const;
    bool isElementPressed(ElementType type, int element, const ControllerVals& vals) const;
    void readController();

    struct StickState {
        Kuribrawl::Vector current_state;
        Kuribrawl::Vector previous_state;
        StickState();
        void updatePrevious();
    };
    struct TriggerState {
        int previous_state;
        int current_state;
        TriggerState();
        void updatePrevious();
    };
    using DpadState = Kuribrawl::Vec2<int8_t>;
    struct ElementsState {
        StickState control_stick;
        StickState secondary_stick; //TODO supporter plusieurs sticks secondaires ?
        TriggerState left_trigger;
        TriggerState right_trigger; /** TODO Supporter un nombre dynamique de triggers ? */
        void updatePrevious();
    };

    const StickState& getControlStickState() const;
    const StickState& getSecondaryStickState() const;
    const TriggerState& getLeftTriggerState() const;
    const TriggerState& getRightTriggerState() const;
    void updateDpadState(); //NOT USED YET
    inline signed char getDpadStateX() const;
    inline signed char getDpadStateY() const;
    const DpadState& getDpadState() const;

    static double normalizeStickValue(int);
    static Kuribrawl::Vec2<double> normalizeStickVector(Kuribrawl::Vector&);

    using ButtonsMapping = std::vector<SDL_GameControllerButton>;

    private:
    bool plugController_(int8_t id);
    bool initButtonMapping();
    void unregisterController();

    //App* app;   ///< The app that opened this Port
    PortsManager& ports_manager;

    int id; /** Index in the \ref app#ports "port vector" */

    //These attributes determine the behavior of the controller or keyboard. They must be set (or unset, as long as everything is valid) everytime the controller changes
    bool active; ///< True if the controller is active ; if false, the other attributes must not be used.
    const ControllerType* controller_type;
    SDL_GameController* controller; ///< MUST BE NULL IF NOT OPEN CURRENTLY
	SDL_Joystick* joystick;
    int joystick_id; ///< Numerical ID of the controller given by SDL, or -1 if using the keyboard.
    const ControllerLayout* current_controller_layout;    ///< The controller layout of the current controller, if any.
    ButtonsMapping controller_buttons_mapping;

    PlayerFighter* fighter; //Pointer validity : is invalidated when the fighter is destroyed, which will happen a lot. The invalidation of this pointer is part of its normal functioning.
    PortOptimizationData pod;

    //State;
    ElementsState elements_state;
    DpadState current_dpad_state; //NOT USED YET

};