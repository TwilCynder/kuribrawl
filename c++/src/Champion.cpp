#include "Champion.h"
#include "Debug.h"
#include "EntityAnimation.h"

#define DEFAULT_GRAVITY 0.6

/**
 * @brief Construct a new Champion:: Champion object
 *
 * @param name_ The internal name.
 */
Champion::Champion(const std::string& name_):
    name(name_),
    default_animations(std::make_unique<const EntityAnimation*[]>((int)DefaultAnimation::TOTAL))
{
    this->val.gravity = DEFAULT_GRAVITY;
}

/**
 * @brief Construct a new Champion::Values object
 * Uses Acid Rainbows as default values.
 */
Champion::Values::Values() :
    walk_speed(4.5), dash_speed(8.0), dash_start_speed(10.0), dash_turn_accel(1.5), traction(0.7),
    max_air_speed(5.0), air_acceleration(0.3), air_friction(0), jump_speed(16), short_hop_speed(8), air_jump_speed(6), gravity(DEFAULT_GRAVITY), max_fall_speed(0), fast_fall_speed(0),
    jump_squat_duration(5), dash_start_duration(-1), dash_stop_duration(-1), dash_turn_duration(-1), landing_duration(-1), guard_start_duration(0), guard_stop_duration(0)
{

}

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

const EntityAnimation* Champion::getDefaultAnimation(const DefaultAnimation anim) const {
    return default_animations[(int)anim];
}

/**
 * @brief Initializes the data relative to the currently added Animations of this Champion.
 * Initializes the state-animation association (based on the \ref Fighter#state_default_animation_name "state-animation name" association),
 * and the callbacks of certains animations based on their key in the map (e.g. the `jump` animation)
 */

void Champion::initAnimations(){
    const EntityAnimation* anim;
    for (auto const& [name, Animation] : animations){
        Debug::log(name);
    }

    for (auto const& [state, name] : default_animation_name){
        if ((anim = getAnimation(name))){
            default_animations[(int)state] = anim;
        }
    }
}

const std::map<Champion::DefaultAnimation, std::string> Champion::default_animation_name = {
    {Champion::DefaultAnimation::IDLE, "idle"},
    {Champion::DefaultAnimation::WALK, "walking"},
    {Champion::DefaultAnimation::WALK_TURN, "walk_turn"},
    {Champion::DefaultAnimation::JUMPSQUAT, "jumpsquat"},
    {Champion::DefaultAnimation::LANDING, "landing"},
    {Champion::DefaultAnimation::DASH, "dash"},
    {Champion::DefaultAnimation::DASH_START, "dash_start"},
    {Champion::DefaultAnimation::DASH_STOP, "dash_stop"},
    {Champion::DefaultAnimation::DASH_TURN, "dash_turn"},
    {Champion::DefaultAnimation::JUMP, "jump"},
    {Champion::DefaultAnimation::AIR_IDLE, "air_idle"}
};