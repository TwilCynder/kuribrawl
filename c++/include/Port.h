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
    struct Stick {
        Kuribrawl::Vector current_state;
        Kuribrawl::Vector previous_state;

        void updatePrevious();
    };

    struct Trigger {
        int previous_state;
        int current_state;

        void updatePrevious();
    };

    public:

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
    const Kuribrawl::Vector& getControlStickState() const;
    const Kuribrawl::Vector& getControlStickPreviousState() const;
    const Kuribrawl::Vector& getSecondaryStickState() const;
    signed char getDpadStateX() const;
    signed char getDpadStateY() const;

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

    //State;
    Stick control_stick;
    Stick secondary_stick; //TODO supporter plusieurs sticks secondaires ?
    Trigger left_trigger;
    Trigger right_trigger; /** TODO Supporter un nombre dynamique de triggers ? */
};