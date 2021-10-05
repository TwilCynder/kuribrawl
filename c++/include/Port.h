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
    void setJoystick(int id);
    void setJoystick(int id, ControllersData& cd);
    void setFighter(PlayerFighter*);
    void initOptimizationData();
    void unregisterController();
    void deactivate();
    void setControllerType(ControllerType* c);
    ControllerType* getController() const;
    void handleButtonPress(int button);
    bool isButtonPressed(int button) const;
    bool isTriggerPressed(int trigger) const;
    bool isElementPressed(ElementType type, int element) const;
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
    void setJoystick_(int id);

    App* app;   ///< The app that opened this Port

    int id;

    //These attributes determine the behavior of the controller or keyboard.
    bool active; ///< True if the controller is active ; if false, the other attributes may be garbage.
    SDLMode sdl_mode;
    ControllerType* controller_type;
    SDL_GameController* controller; ///< MUST BE NULL IF NOT OPEN CURRENTLY
	SDL_Joystick* joystick;
    int joystick_id; ///< Numerical ID or the controller given by SDL, or -1 if using the keyboard.

    PlayerFighter* fighter; //Pointer validity : is invalidated when the fighter is destroyed, which will happen a lot. The invalidation of this pointer is part of its normal functioning.

    PortOptimizationData pod;
    DpadState current_dpad_state; //NOT USED YET

    //State;
    StickState control_stick;
    StickState secondary_stick; //TODO supporter plusieurs sticks secondaires ?
    TriggerState left_trigger;
    TriggerState right_trigger; /** TODO Supporter un nombre dynamique de triggers ? */
};