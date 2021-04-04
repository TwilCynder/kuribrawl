#pragma once
#include <map>
#include <string>
#include "SDL2/SDL.h"
#include "CurrentAnimation.h"
#include "InputManager.h"
#include "util.h"

class Port;
class Champion;

/**
 * @brief In-game character ("instance" of a Champion)
 * 
 */
class Fighter {
    public:

    enum class State {
        IDLE,
        GUARD,
        GUARD_START,
        GUARD_STOP,
        WALK,
        WALK_TURN,
        DASH,
        DASH_START,
        DASH_STOP,
        DASH_TURN,
        JUMPSQUAT,
        LANDING,
        ATTACK,
        HITSTUN,
        TUMBLE,
        GUARD_STUN,
        LANDING_LAG,
        CROUCH,
        CROUCH_START,
        CROUCH_STOP,
        STATES
    };

    Fighter(Champion* model_);
    Fighter(Champion* model_, int x, int y);

    void draw(SDL_Renderer* target);
    //Physics
    void applyPhysics();
    //Inputs
    void updateInputs();
    //States
    void updateState();
    void updateAnimation();

    State getState() const;
    void setState(const State s, int facing = 0, int info = 0, bool update_anim_ = true);
    CurrentAnimation* getCurrentAnimation();
    Kuribrawl::Vector* getPosition();
    void setSpeed(double x, double y);
    bool getGrounded() const;


    InputManager* getInputManager() const;
    Port* getPort() const;
    void setPort(Port* port);

    bool is_initialized();

    static const std::map<State, std::string> state_default_animation_name; 

    private:
    Champion* model;    //Pointer validity : can be invalidated if a champion is deleted or moved (should not happen while a Fighter instance exists)

    CurrentAnimation current_animation;
    std::string current_animation_name;

    Kuribrawl::Vector position;
    Kuribrawl::VectorDouble speed;
    Port* port;     ////Pointer validity : dla merde
    std::unique_ptr<InputManager> input_manager;

    State state;
    int state_info;
    int state_timer;
    bool update_anim;
    bool paused;
    int facing;
    bool grounded;

    //Inputs
    void checkStickState();

    //Physics
    void groundCollision();
    void groundToAir();
};