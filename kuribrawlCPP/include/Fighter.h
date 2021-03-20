#pragma once
#include <map>
#include <string>
#include "SDL2/SDL.h"
#include "CurrentAnimation.h"
#include "Champion.h"
#include "InputManager.h"
#include "util.h"

class Port;

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
        CROUCH_STOP
    };

    Fighter(Champion* model_);
    Fighter(Champion* model_, int x, int y);

    void draw(SDL_Renderer* target, int x, int y);
    CurrentAnimation* getCurrentAnimation();
    const Kuribrawl::Vector* getPosition();
    
    State getState() const;
    void setState(const State s, int info, bool update_anim_);

    bool is_initialized();

    static const std::map<State, std::string> state_default_animation_name; 

    private:
    Champion* model;
    CurrentAnimation current_animation;
    std::string current_animation_name;
    Kuribrawl::Vector position;
    Kuribrawl::Vector speed;
    Port* port;
    std::unique_ptr<InputManager> input_manager;
    State state;
    int state_info;
    bool update_anim;
};