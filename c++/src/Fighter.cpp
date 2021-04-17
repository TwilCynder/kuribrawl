#include "Debug.h"
#include "DebugState.h"
#include "Fighter.h"
#include "Port.h"
#include "defs.h"
#include "util.h"
#include "Champion.h"
#include "InputManager.h"
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
 * @brief Construct a new Fighter:: Fighter object
 * 
 * @param model_ 
 * @param x_ 
 * @param y_ 
 */
Fighter::Fighter(Champion* model_, int x_, int y_):
    model(model_),
    port(nullptr),
    input_manager(std::make_unique<InputManager>(this)),
    state(State::IDLE),
    paused(false),
    facing(1),
    grounded(false)
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

bool Fighter::is_initialized(){
    return current_animation.is_initialized();
}

Kuribrawl::VectorDouble& Fighter::getPosition() {
    return position;
}

CurrentAnimation* Fighter::getCurrentAnimation(){
    return &current_animation;
}

void Fighter::setSpeed(double x, double y){
    speed.x = x;
    speed.y = y;
}

bool Fighter::getGrounded() const{
    return grounded;
}

void Fighter::draw(SDL_Renderer* target){
    current_animation.draw(target, position.x , SCREEN_HEIGHT - position.y);
}

void Fighter::checkStickState(){ //lots of error checks to do
    Kuribrawl::Vector control_stick_state = port->getControlStickState();
    ControllerType::ControllerVals controller_vals = port->getController()->getControllerVals();
    switch (state){
        case State::IDLE:
            if (abs(control_stick_state.x) > controller_vals.analogStickThreshold){
                if (grounded){
                    setState(State::WALK, Kuribrawl::sign(control_stick_state.x));
                }
            }
            break;
        case State::WALK:
            if (abs(control_stick_state.x) < controller_vals.analogStickThreshold){
                setState(State::IDLE);
            } else if (Kuribrawl::sign(control_stick_state.x) != facing){
                
                setState(State::WALK, -facing, 0, false);
            }
            break;
        default:
            break;
    }
}

void Fighter::updateInputs(){
    checkStickState();
    input_manager->updateInputs();
}

void Fighter::updateState(){
    ++state_timer; 
}

void Fighter::updateAnimation(){
    if (update_anim){
        Animation* anim = model->getStateAnimation(state);
        if (anim){
            current_animation.setAnimation(anim);
        }
        update_anim = false;
    }
}

InputManager* Fighter::getInputManager() const{
    return input_manager.get();
}

Port* Fighter::getPort() const {
    return port;
}

void Fighter::setPort(Port* port_){
    port = port_;
}

Fighter::State Fighter::getState() const {
    return state;
}

void Fighter::setState(const Fighter::State s, int facing_, int info, bool update_anim_){
    state = s;
    state_info = info;
    update_anim = update_anim_;
    if (facing_) facing = facing_;
}

const std::map<Fighter::State, std::string> Fighter::state_default_animation_name = {
    {Fighter::State::IDLE, "idle"},
    {Fighter::State::WALK, "walking"},
    {Fighter::State::WALK_TURN, "walk_turn"},
    {Fighter::State::JUMPSQUAT, "jumpsquat"},
    {Fighter::State::LANDING, "landing"}
};