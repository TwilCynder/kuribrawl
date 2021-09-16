#pragma once

#include <forward_list>
#include "PlayerFighter.h"
#include "ControllerType.h"
#include "inputs.h"
#include "util.h"
#include "PortOptimizationData.h"

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

    Port(App* app_);

    bool isActive() const ;
    void setJoystick(int id);
    void setJoystick(int id, ControllerType* controller);
    void setJoystick(int id, ControllersData& cd);
    void setFighter(PlayerFighter*);
    void initOptimizationData();
    void deactivate();
    void setController(ControllerType* c);
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

    App* app;

    ControllerType* controller_type;
    PlayerFighter* fighter; //Pointer validity : is invalidated when the fighter is destroyed, which will happen a lot. The invalidation of this pointer is part of it's normal functioning.

    int id;
    int joystick_id;
    SDL_GameController* controller; ///< MUST BE NULL IF NOT OPEN CURRENTLY
	SDL_Joystick* joystick;
    bool active;

    PortOptimizationData pod;
    DpadState current_dpad_state; //NOT USED YET

    //State;
    StickState control_stick;
    StickState secondary_stick; //TODO supporter plusieurs sticks secondaires ?
    TriggerState left_trigger;
    TriggerState right_trigger; /** TODO Supporter un nombre dynamique de triggers ? */
};