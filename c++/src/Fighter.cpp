#include "Debug.h"
#include "DebugState.h"
#include "Fighter.h"
#include "defs.h"
#include "util.h"
#include "Champion.h"
#include <math.h>

/**
 * @brief Construct a new Fighter at position 0, 0
 * Creates a Fighter at position 0,0
 * @param model_ : the Champion used as a model
 */
Fighter::Fighter(Champion* model_):
    Fighter(model_, 0, 0)
{
}

/**
 * @brief Construct a new Fighter object
 * 
 * @param model_ the Champion used as a model
 * @param x_ the x position on the stage thet Fighter will spawn at.
 * @param y_ the y position on the stage thet Fighter will spawn at.
 */
Fighter::Fighter(Champion* model_, int x_, int y_):
    state(State::IDLE),
    paused(false),
    facing(1),
    grounded(false),
    model(model_)
{
    Animation* idle_anim = model->getAnimation("idle");

    if (!idle_anim){
        throw KBFatal("Tried to instanciate a champion with no idle animation");
    }

    current_animation.setAnimation(idle_anim);

    position.x = x_;
    position.y = y_;
    speed.x = 0.0;
    speed.y = 0.0;
}

/**
 * @brief Returns whether the Fighter is correctly initialized and ready to use.
 * 
 * @return true is the CurrentAnimation is ready to use.
 * @return false otherwise.
 */
bool Fighter::is_initialized(){
    return current_animation.is_initialized();
}

/**
 * @brief Return a vector representing the current position of this Fighter on the Stage.
 * 
 * @return Kuribrawl::VectorDouble& 
 */
Kuribrawl::VectorDouble& Fighter::getPosition() {
    return position;
}

/**
 * @brief Returns the Animation of the Champion that is being displayed currently.
 * 
 * @return CurrentAnimation* a pointer to the Animation the CurrentAnimation is currently playing.
 */
CurrentAnimation* Fighter::getCurrentAnimation(){
    return &current_animation;
}

/**
 * @brief Sets the x and y components of the speed of this Fighter
 * @param x x speed in pixels per frame
 * @param y y speed in pixels per frame
 */
void Fighter::setSpeed(double x, double y){
    speed.x = x;
    speed.y = y;
}

/**
 * @brief Returns whether this Fighter is on the ground.
 * @return true if this Fighter is on top of a walkable object.
 * @return false otherwise
 */
bool Fighter::getGrounded() const{
    return grounded;
}


/**
 * @brief displays the Fighter.
 * Simply displays the CurrentAnimation.
 * @param target the renderer the frame will be drawn to.
 */
void Fighter::draw(SDL_Renderer* target){
    Debug::log(state);
    current_animation.draw(target, position.x , SCREEN_HEIGHT - position.y);
}

/**
 * @brief Makes the Fighter jump.
 * 
 */
void Fighter::jump(jumpX x_type, jumpY y_type){
    switch (y_type){
        case jumpY::Full:
            speed.y += model->val.jump_speed;
            break;
        case jumpY::Short:
            speed.y += model->val.short_hop_speed;
            break;
        case jumpY::Air:
            speed.y += model->val.air_jump_speed;
            break;
        default:
            break;
    }
    setState(State::IDLE, 0, 1);
}

/**
 * @brief If the current state is limited in time, returns whether is has reached its end.
 * 
 * @param duration value used as the duration (usually one of the Champion's fixed value)
 * @return true if the duration is -1 and the current animation was finished, or if the timer state has reached the duration.
 */
bool Fighter::isStateFinished(int duration){
    return (duration == -1) ? current_animation.is_finished() : state_timer >= duration;
}

/**
 * @brief Checks if any change of state should be made based on the current state and the \ref Fighter#state_time "state timer".
 * 
 */
void Fighter::updateState(){
    if (paused) return;
    ++state_timer;
    switch (state){
        case State::JUMPSQUAT:
            if (isStateFinished(model->val.jump_squat_duration)){
                jump(jumpX::Normal, jumpY::Full);
            }
            break;
        default:
            break;
    }
}

/**
 * @brief If this current Animation should be updated, changes it based on the current state.
 * Also indicates that the current Animation doesn't need to be updated anymore.
 */
void Fighter::updateAnimation(){
    if (update_anim){
        switch(state){
            case State::IDLE:
                break;
            default:
                Animation* anim = model->getStateAnimation(state);
                if (anim){
                    current_animation.setAnimation(anim);
                }
        }
        update_anim = false;
    }
}

/**
 * @brief Returns the current \ref Fighter#State "state" of this Fighter.
 * 
 * @return Fighter::State
 */

Fighter::State Fighter::getState() const {
    return state;
}

/**
 * @brief Sets the current state of this Fighter.
 * @param s a Fighter::State.
 * @param facing_ the \ref Fighter#direction" direction" the Fighter will be facing (0 is the default value means the same as before)
 * @param info the new value of \ref Fighter#state_info "state_info". (defaults to 0, the value is overridden even if null)
 * @param update_anim_ Whether the current Animation should be changed according to the new state as soon as possible (defaults to true);
 */

void Fighter::setState(const Fighter::State s, int facing_, int info, bool update_anim_){
    state = s;
    state_info = info;
    update_anim = update_anim_;
    if (facing_) facing = facing_;

    state_timer = 0;
}

const std::map<Fighter::State, std::string> Fighter::state_default_animation_name = {
    {Fighter::State::IDLE, "idle"},
    {Fighter::State::WALK, "walking"},
    {Fighter::State::WALK_TURN, "walk_turn"},
    {Fighter::State::JUMPSQUAT, "jumpsquat"},
    {Fighter::State::LANDING, "landing"}
};