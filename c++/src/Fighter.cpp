#include "Debug.h"
#include "DebugState.h"
#include "Fighter.h"
#include "defs.h"
#include "util.h"
#include "Champion.h"
#include "CollisionBoxes.h"
#include "Move.h"
#include "Random.h"
#include <math.h>

/**
 * @brief Construct a new Fighter at position 0, 0
 * Creates a Fighter at position 0,0
 * @param model_ : the Champion used as a model
 */
Fighter::Fighter(Game& game, Champion* model_):
    Fighter(game, model_, 0, 0)
{
}

/**
 * @brief Construct a new Fighter object
 *
 * @param model_ the Champion used as a model
 * @param x_ the x position on the stage thet Fighter will spawn at.
 * @param y_ the y position on the stage thet Fighter will spawn at.
 */
Fighter::Fighter(Game& game_, Champion* model_, int x_, int y_):
    state(State::IDLE),
    update_anim(true),
    paused(false),
    facing(1),
    grounded(false),
	air_jumps(model_->val.air_jumps),
    model(model_),
    game(game_),
    current_move(nullptr)
{
    position.x = x_;
    position.y = y_;
    speed.x = 0.0;
    speed.y = 0.0;

    id = Random::get_int();
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

void Fighter::setAnimation(Champion::DefaultAnimation default_anim){
    current_animation.setAnimation(model->getDefaultAnimation(default_anim));
}

void Fighter::setAnimation(Champion::DefaultAnimation default_anim, double speed){
    current_animation.setAnimation(model->getDefaultAnimation(default_anim), speed);
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

int Fighter::getFacing() const{
    return facing;
}

/**
 * @brief displays the Fighter.
 * Simply displays the CurrentAnimation.
 * @param target the renderer the frame will be drawn to.
 */
void Fighter::draw(SDL_Renderer* target){
    current_animation.draw(target, position.x , SCREEN_HEIGHT - position.y, facing);

    SDL_Rect box;

    //Drawing hurtboxes
    const std::vector<Hurtbox>& hurtboxes = current_animation.getHurtboxes();
    SDL_SetRenderDrawColor(target, 0, 255, 0, 255);
    for (unsigned int i = 0; i < hurtboxes.size(); i++){
        box.w = hurtboxes[i].w;
        box.h = hurtboxes[i].h;
        box.x = this->position.x + hurtboxes[i].getRealXPos(facing);
        box.y = SCREEN_HEIGHT - (this->position.y + hurtboxes[i].y);
        SDL_RenderDrawRect(target, &box);
    }

    //Drawing hitboxes
    const std::vector<Hitbox>& hitboxes = current_animation.getHitboxes();
    SDL_SetRenderDrawColor(target, 255, 0, 0, 255);
    for (unsigned int i = 0; i < hitboxes.size(); i++){
        box.w = hitboxes[i].w;
        box.h = hitboxes[i].h;
        box.x = this->position.x + hitboxes[i].getRealXPos(facing);
        box.y = SCREEN_HEIGHT - (this->position.y + hitboxes[i].y);
        SDL_RenderDrawRect(target, &box);
    }

    SDL_SetRenderDrawColor(target, 0, 0, 255, 255);
    SDL_RenderDrawLine(target, position.x - 10, SCREEN_HEIGHT - position.y, position.x + 10, SCREEN_HEIGHT - position.y);
    SDL_RenderDrawLine(target, position.x, SCREEN_HEIGHT - position.y - 10, position.x, SCREEN_HEIGHT - position.y + 10);
}

/**
 * @brief Makes the Fighter jump.
 *
 */
void Fighter::ground_jump(jumpX x_type, jumpY y_type){
	x_type = (x_type == jumpX::UndecidedX) ? decideJumpXType() : x_type;
	y_type = (y_type == jumpY::UndecidedY) ? decideGroundedJumpYType() : y_type;

    switch (y_type){
        case jumpY::Full:
            speed.y += model->val.jump_speed;
            break;
        case jumpY::Short:
            speed.y += model->val.short_hop_speed;
            break;
        default:
            break;
    }
    
    switch(x_type){
        case jumpX::Forward:
            if (speed.x * facing < model->val.ground_forward_jump_speed){
                speed.x = model->val.ground_forward_jump_speed * facing;
            }
            break;
        case jumpX::Backwards:
            if (speed.x * -facing < model->val.ground_backward_jump_speed){
                speed.x = model->val.ground_backward_jump_speed * -facing;
            }
            break;
        default:
            break;
    }

    setState(State::IDLE, 0, 1, false);
    setAnimation(Champion::DefaultAnimation::JUMP);
    grounded = false;
}


//Returns int to make jump_manager able to return its return value directly
int Fighter::air_jump(jumpX x_type){
	Debug::log("air jump ????");
	if (air_jumps > 0) {    
		x_type = (x_type == jumpX::UndecidedX) ? decideJumpXType() : x_type;
	
	    switch(x_type){
            case jumpX::Forward:
                if (speed.x * facing < model->val.air_forward_jump_speed){
                    speed.x = model->val.air_forward_jump_speed * facing;
                }
                break;
            case jumpX::Backwards:
                if (speed.x * -facing < model->val.air_backward_jump_speed){
                    speed.x = model->val.air_backward_jump_speed * -facing;
                }
                break;
            default:
                break;
        }
		
        speed.y = model->val.air_jump_speed;
		air_jumps--;
		setState(Fighter::State::IDLE, 0, 0, false);
        setAnimation(Champion::DefaultAnimation::AIR_JUMP);
        
        return 1;
	} else {
		return 0;
	}

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

void Fighter::checkStateDuration(){
    switch (state){
        case State::JUMPSQUAT:
            if (!model->val.jump_squat_duration){
                ground_jump();
            }
            break;
        case State::DASH_START:
            if (!model->val.dash_start_duration){
                setState(State::DASH);
            }
            break;
        case State::DASH_STOP:
            if (!model->val.dash_stop_duration){
                setState(State::IDLE);
            }
            break;
        case State::DASH_TURN:
            if (!model->val.dash_turn_duration){
                setState(State::DASH);
            }
            break;
        case State::LANDING:
            if (!model->val.landing_duration){
                setState(State::IDLE);
            }
            break;

        default:
            break;
    }
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
                ground_jump();
            }
            break;
        case State::DASH_START:
            if (isStateFinished(model->val.dash_start_duration)){
                setState(State::DASH);
            }
            break;
        case State::DASH_STOP:
            if (isStateFinished(model->val.dash_stop_duration)){
                setState(State::IDLE);
            }
            break;
        case State::DASH_TURN:
            if (isStateFinished(model->val.dash_turn_duration)){
                setState(State::DASH);
            }
            break;
        case State::LANDING:
            if (isStateFinished(model->val.landing_duration)){
                setState(State::IDLE);
            }
            break;
        case State::ATTACK:
            if (current_animation.is_finished()){
                switch (current_move->end_behavior){
                    case Move::EndBehavior::NORMAL:
                        setState(Fighter::State::IDLE);
                        break;
                    case Move::EndBehavior::FREEFALL:
                        setState(Fighter::State::FREEFALL);
                }
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
        const EntityAnimation* anim;
        switch(state){
            case State::IDLE:
                anim = (grounded) ? 
                    model->getDefaultAnimation(Champion::DefaultAnimation::IDLE) : 
                    (state_info == 1) ? model->getDefaultAnimation(Champion::DefaultAnimation::JUMP) : model->getDefaultAnimation(Champion::DefaultAnimation::AIR_IDLE);
                if (anim)
                    current_animation.setAnimation(anim);
                break;
            default:
                anim = model->getDefaultAnimation((Champion::DefaultAnimation)state);
                if (anim)
                    current_animation.setAnimation(anim);
        }
        update_anim = false;
    }
}

const HurtboxVector& Fighter::getCurrentHurtboxes() const{
    return current_animation.getHurtboxes();
}

const HitboxVector& Fighter::getCurrentHitboxes() const{
    return current_animation.getHitboxes();
}

/**
 * @brief Returns the game
 * 
 */

Game& Fighter::getGame(){
    return game;
}

/**
 * @brief Returns the Champions this Fighter is an instance of
 * 
 * @return Champion& 
 */
const Champion& Fighter::getChampion(){
    return *model;
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
    Debug::log(s, *this);
    state = s;
    state_info = info;
    update_anim = update_anim_;
    if (facing_) facing = facing_;

    state_timer = 0;
    checkStateDuration();
}

void Fighter::startMove(const Move& move){
    current_move = &move;
    if (move.animation){
        current_animation.setAnimation(move.animation);
    }
}

void Fighter::attack(const Move& move){
    setState(Fighter::State::ATTACK);
    startMove(move);
}