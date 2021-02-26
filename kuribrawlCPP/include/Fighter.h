#pragma once
#include <map>
#include <string>

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

    static const std::map<State, std::string> state_default_animation; 

    private:
};