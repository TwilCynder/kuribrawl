#pragma once

#include <forward_list>
#include "Fighter.h"
#include "ControllerType.h"
#include "Binding.h"
#include "inputs.h"
#include "util.h"

#define CONTROL_STICK_FRAME_BUFFER 2 //TODO make it customizable ?

class App;

class Port {
    struct Stick {
        Kuribrawl::Vector current_state;
        Kuribrawl::Vector previous_state;

        void updatePrevious();
    };

    public:

    Port(App* app_);
    ~Port();

    bool isActive() const ;
    void setJoystick(int id);
    void setJoystick(int id, ControllerType* controller);
    void setFighter(Fighter*);
    void deactivate();
    void setController(ControllerType* c);
    ControllerType* getController() const;
    Binding* getInputBinding() const;
    void handleButtonPress(int button);
    bool isButtonPressed(int button);
    void readController();
    void updateInputs();
    const Kuribrawl::Vector& getControlStickState() const;
    const Kuribrawl::Vector& getSecondaryStickState() const;

    int debug;

    private:
    void swap_control_stick_buffers();
    void setJoystick_(int id);

    App* app;

    ControllerType* controller;
    Fighter* fighter; //Pointer validity : is invalidated when the fighter is destroyed, which will happen a lot
    Binding* input_binding;

    int id;
    int joystick_id;
    SDL_GameController* joystick;
    bool active;

    //State;
    Stick control_stick;
    Stick secondary_stick; //TODO supporter plusieurs sticks secondaires ?
    int* control_stick_buffer[2]; //contains pointers to 4-int arrays, each int representing a direction, each array represent a previous frame. Most recent first
};