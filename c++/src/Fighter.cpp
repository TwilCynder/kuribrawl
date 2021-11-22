#include "Debug.h"
#include "DebugState.h"
#include "Fighter.h"
#include "defs.h"
#include "util.h"
#include "Champion.h"
#include "CollisionBoxes.h"
#include "Move.h"
#include "Random.h"
#include "gameCalculations.h"
#include "Text/TextDisplayer.h"
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
    current_move(nullptr),
    last_hit(-1)
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
 * @return true is the AnimationPlayer is ready to use.
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
 * @return Animation* a pointer to the Animation the AnimationPlayer is currently playing.
 */
const EntityAnimation* Fighter::getCurrentAnimation() const {
    return current_animation.getAnimation();
}

void Fighter::setAnimation(const EntityAnimation* anim) {
    last_hit = -1;
    current_animation.setAnimation(anim);
}

void Fighter::setAnimation(const EntityAnimation* anim, double speed) {
    last_hit = -1;
    current_animation.setAnimation(anim, speed);
}


///\todo Use templates.
void Fighter::setAnimation(Champion::DefaultAnimation default_anim){
    const EntityAnimation* anim = model->getDefaultAnimation(default_anim);
    checkNull(anim, KBFatalExplicit("Missing Animation"))
    setAnimation(anim);
}

void Fighter::setAnimation(Champion::DefaultAnimation default_anim, double speed){
    const EntityAnimation* anim = model->getDefaultAnimation(default_anim);
    checkNull(anim, KBFatalExplicit("Missing Animation"))
    setAnimation(anim, speed);
}

bool Fighter::setAnimationMaybe(Champion::DefaultAnimation default_anim){
    const EntityAnimation* anim = model->getDefaultAnimation(default_anim);
    if (!anim) return false;
    setAnimation(anim);
    return true;
}

bool Fighter::setAnimationMaybe(Champion::DefaultAnimation default_anim, double speed){
    const EntityAnimation* anim = model->getDefaultAnimation(default_anim);
    if (!anim) return false;
    setAnimation(anim, speed);
    return true;
}

void Fighter::advanceAnimation(){
    current_animation.advance();
}

void Fighter::applyFrameMovement(double& speed, const EntityFrame::FrameMovementAxis& fma){
    if (fma.enabled){
        if (fma.whole_frame){
            speed = fma.value + (fma.set_speed ? 0 : speed); 
        } else if (current_animation.frameChanged()){
            speed = fma.value + (fma.set_speed ? 0 : speed); 
        }
    }
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

bool Fighter::hitFighter(Fighter& defender, const Hitbox& hitbox, const Hurtbox& hurtbox){
    if (last_hit != hitbox.hit){
        fighters_hit.clear();
        last_hit = hitbox.hit;
    } else {
        for (Fighter* f : fighters_hit){
            if (f == &defender) return false;
        }
    }

    fighters_hit.add(&defender);
    return true;
}

void Fighter::getHit(Fighter& attacker, const Hitbox& hitbox, const Hurtbox& hurtbox) {
    double knockback = GameCalc::getKnockback(0, 0, 0, 0);
    double angle = PI * 3 / 8;
    if (attacker.facing < 0){
        angle = (PI - angle);
    }

    speed.x = knockback * cos(angle);
    speed.y = knockback * sin(angle);

    int hitstun = 10;
    setState(Fighter::State::HITSTUN, -sign(speed.x), hitstun, false);

    /*Choix de l'animation de hitstun*/
    setAnimation(Champion::DefaultAnimation::HITSTUN);
}

/**
 * @brief displays the Fighter.
 * Simply displays the AnimationPlayer.
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

void Fighter::drawDebugInfo(AdvancedTextDisplayer& out){
    out 
        << "Speed X : " << speed.x << AdvancedTextDisplayer::force_advance(80) 
        << " ; Speed Y : " << speed.y << AdvancedTextDisplayer::force_advance(80)
        << " " << Debug::state_to_string(state);
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
        case State::HITSTUN:    //State info is the hitstun duration
            if (state_timer >= state_info) {
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
                        break;
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
void Fighter::checkUpdateAnimation(){
    if (update_anim){
        updateAnimation();
        update_anim = false;
    }
}

void Fighter::updateAnimation(){
    const EntityAnimation* anim;
    switch(state){
        case State::IDLE:
            anim = (grounded) ? 
                model->getDefaultAnimation(Champion::DefaultAnimation::IDLE) : 
                (state_info == 1) ? model->getDefaultAnimation(Champion::DefaultAnimation::JUMP) : model->getDefaultAnimation(Champion::DefaultAnimation::AIR_IDLE);
            if (anim)
                current_animation.setAnimation(anim);
            break;
        case State::HITSTUN:
            break;
        default:
            anim = model->getDefaultAnimation((Champion::DefaultAnimation)state);
            if (anim)
                current_animation.setAnimation(anim);
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

void Fighter::setState(const Fighter::State s, int facing, int info, Champion::DefaultAnimation anim){
    setAnimation(anim);
    setState(s, facing, info, false);
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