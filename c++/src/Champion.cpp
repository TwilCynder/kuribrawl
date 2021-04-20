#include "Champion.h"
#include "Debug.h"
#include "Animation.h"

#define DEFAULT_GRAVITY 0.6

/**
 * @brief Construct a new Champion:: Champion object
 * 
 * @param name_ The internal name.
 */
Champion::Champion(const std::string& name_):
    name(name_),
    state_animations(std::make_unique<Animation*[]>((int)Fighter::State::STATES))
{
    this->val.gravity = DEFAULT_GRAVITY;
}

Champion::Values::Values() : 
    walk_speed(2.0), dash_speed(0), dash_start_speed(0), dash_turn_accel(0), traction(0.05), 
    max_air_speed(0), air_acceleration(0), air_friction(0), jump_speed(8), short_hop_speed(4), air_jump_speed(6), gravity(DEFAULT_GRAVITY), max_fall_speed(0), fast_fall_speed(0),
    jump_squat_duration(5), dash_start_duration(0), dash_stop_duration(0), dash_turn_duration(0), landing_duration(0), guard_start_duration(0), guard_stop_duration(0)
{}

/**
 * @brief Returns the internal name
 * 
 * @return const std::string& internal string identifier.
 */

const std::string& Champion::getName(){
    return name;
}

/**
 * @brief Returns the Animation associated with a certain \ref Fighter#State "fighter state", if there is any.
 * 
 * @param state 
 * @return Animation* a pointer to the Animation. Can be NULL.
 */

Animation* Champion::getStateAnimation(const Fighter::State state) const {
    return state_animations[(int)state];
}

/**
 * @brief Initializes the data relative to the currently added Animations of this Champion.
 * Initializes the state-animation association (based on the \ref Fighter#state_default_animation_name "state-animation name" association), 
 * and the callbacks of certains animations based on their key in the map (e.g. the `jump` animation)
 */

void Champion::initAnimations(){
    Animation* anim;
    for (auto const& [state, name] : Fighter::state_default_animation_name){
        if ((anim = getAnimation(name))){
            state_animations[(int)state] = anim;
        }
    }
}