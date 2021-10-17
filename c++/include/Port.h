#pragma once

#include <forward_list>
#include "PlayerFighter.h"
#include "ControllerType.h"
#include "inputs.h"
#include "util.h"
#include "PortOptimizationData.h"

#define JOSTICKID_KEYBOARD -1

class App;
class Binding;
class ControllersData;

class Port {
    public:
    struct StickState {
        Kuribrawl::Vector current_state;
        Kuribrawl::Vector previous_state;

        void updatePrevious();
    };

    struct TriggerState {
        int previous_state;
        int current_state;

        void updatePrevious();
    };

    enum class SDLMode {
        JOYSTICK,
        GAMECONTROLLER,
        INVALID
    };

    Port(App* app_);

    bool isActive() const;
    void plugController(int id, ControllersData& cd);
    void setFighter(PlayerFighter*);
    void initOptimizationData();
    void deactivate();
    void setControllerType(const ControllerType* c);
    const ControllerType* getControllerType() const;

    void handleButtonPress(int button);
    bool isButtonPressed(int button) const;
    bool isJoystickButtonPressed(int button) const;
    bool isTriggerPressed(int trigger) const;
    bool isTriggerPressed(int trigger, const ControllerVals& vals) const;
    bool isElementPressed(ElementType type, int element) const;
    bool isElementPressed(ElementType type, int element, const ControllerVals& vals) const;
    void readController();
    const StickState& getControlStickState() const;
    const StickState& getSecondaryStickState() const;
    const TriggerState& getLeftTriggerState() const;
    const TriggerState& getRightTriggerState() const;
    void updateDpadState(); //NOT USED YET
    inline signed char getDpadStateX() const;
    inline signed char getDpadStateY() const;
    using DpadState = Kuribrawl::VectorT<int8_t>;
    const DpadState& getDpadState() const;

    private:
    void plugController_(int id);
    void unregisterController();

    App* app;   ///< The app that opened this Port

    int id; /** Index in the \ref app#ports "port vector" */

    //These attributes determine the behavior of the controller or keyboard. They must be set (or unset, as long as everything is valid) everytime the controller changes
    bool active; ///< True if the controller is active ; if false, the other attributes must not be used.
    const ControllerType* controller_type;
    SDL_GameController* controller; ///< MUST BE NULL IF NOT OPEN CURRENTLY
	SDL_Joystick* joystick;
    int joystick_id; ///< Numerical ID of the controller given by SDL, or -1 if using the keyboard.
    const ControllerLayout* current_controller_layout;    ///< The controller layout of the current controller, if any.

    PlayerFighter* fighter; //Pointer validity : is invalidated when the fighter is destroyed, which will happen a lot. The invalidation of this pointer is part of its normal functioning.
    PortOptimizationData pod;

    //State;
    StickState control_stick;
    StickState secondary_stick; //TODO supporter plusieurs sticks secondaires ?
    TriggerState left_trigger;
    TriggerState right_trigger; /** TODO Supporter un nombre dynamique de triggers ? */
    DpadState current_dpad_state; //NOT USED YET
};