#include <math.h>
#include "Fighter.h"
#include "defs.h"
#include "Util/util.h"
#include "Util/Drawing.h"
#include "Champion.h"
#include "CollisionBoxes.h"
#include "Move.h"
#include "Random.h"
#include "gameCalculations.h"

#include "Util/Text/TextDisplayer.h"
#include "Camera.h"

#include "KBDebug/Debug.h"
#include "KBDebug/DebugState.h"
#include "Display/DisplayText.h"

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
    //update_anim(true),
    paused(false),
    ground_interaction(GroundInteraction::NONE),
    facing(Kuribrawl::Side::NEUTRAL),
    grounded(false),
	air_jumps(model_->values.air_jumps),
    current_ground(nullptr),
    model(model_),
    game(game_),
    current_move(nullptr),
    last_hit(-1)
{
    position.x = x_;
    position.y = y_;
    speed.x = 0.0;
    speed.y = 0.0;

    setState(State::IDLE, Kuribrawl::Side::LEFT);

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
const Kuribrawl::VectorDouble& Fighter::getPosition() const {
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

void Fighter::changeAnimation(const EntityAnimation* anim){
    const EntityAnimation* transition = model->getAnimationTransition(*current_animation.getAnimation(), *anim);


    if (transition){   
        current_animation.setAnimation(transition, EntityAnimationEndAction_(anim));
    } else {
        current_animation.setAnimation(anim);
    }
}

void Fighter::changeAnimation(const EntityAnimation* anim, double speed){
    const EntityAnimation* transition = model->getAnimationTransition(*current_animation.getAnimation(), *anim);
    
    if (speed < 0){
        if (transition){
            current_animation.setAnimation(transition, EntityAnimationEndAction_(anim));
        } else {
            current_animation.setAnimation(anim);
        }
    } else {
        if (transition){
            current_animation.setAnimation(transition, speed, EntityAnimationEndAction_(anim));
        } else {
            current_animation.setAnimation(anim, speed);
        }
    }


}

void Fighter::setAnimation(const EntityAnimation* anim) {
    last_hit = -1;
    if (anim)
        changeAnimation(anim);
    else
        throw KBFatal("Attempting to set null animation");
}

void Fighter::setAnimation(const EntityAnimation* anim, double speed) {
    last_hit = -1;
    if (anim)
        changeAnimation(anim, speed);
    else
        throw KBFatal("Attempting to set null animation");
}


///\todo Use templates.
void Fighter::setAnimation(Champion::DefaultAnimation default_anim){
    const EntityAnimation* anim = model->getDefaultAnimation(default_anim);
    checkNull(anim, KBFatalExplicit("Missing Animation"))
    changeAnimation(anim);
}

void Fighter::setAnimation(Champion::DefaultAnimation default_anim, double speed){
    const EntityAnimation* anim = model->getDefaultAnimation(default_anim);
    checkNull(anim, KBFatalExplicit("Missing Animation"))
    changeAnimation(anim, speed);
}

bool Fighter::setAnimationMaybe(Champion::DefaultAnimation default_anim){
    const EntityAnimation* anim = model->getDefaultAnimation(default_anim);
    if (!anim) return false;
    changeAnimation(anim);
    return true;
}

bool Fighter::setAnimationMaybe(Champion::DefaultAnimation default_anim, double speed){
    const EntityAnimation* anim = model->getDefaultAnimation(default_anim);
    if (!anim) return false;
    changeAnimation(anim, speed);
    return true;
}

void Fighter::advanceAnimation(){
    if (paused) return;
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

Kuribrawl::Side Fighter::getFacing() const{
    return facing;
}

bool Fighter::hitFighter(Fighter& defender, const Hitbox& hitbox, const Hurtbox& hurtbox){
    if (last_hit < hitbox.hit){
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
    if (state  == State::HITSTUN) Debug::log("Combo !");

    double knockback = GameCalc::getKnockback(0, 0, 0, 0) * 2;
    double angle = PI * 3 / 8;
    if (attacker.facing == Kuribrawl::Side::LEFT){
        angle = (PI - angle);
    }

    speed.x = knockback * cos(angle);
    speed.y = knockback * sin(angle);

    int hitstun = 10;
    setState(Fighter::State::HITSTUN, Kuribrawl::signToSide(-speed.x), hitstun, false);

    /*Choix de l'animation de hitstun*/
    setAnimation(Champion::DefaultAnimation::HITSTUN);

    //Hitlag
    int hitLag = GameCalc::getHitLag(hitbox.damage);
    attacker.paused = hitLag;
    paused = hitLag;
}

/**
 * @brief displays the Fighter.
 * Simply displays the AnimationPlayer.
 * @param target the renderer the frame will be drawn to.
 */
void Fighter::draw(SDL_Renderer* target, const Camera& cam) const{
    current_animation.draw(target, cam.getXOnScreen(position.x) , cam.getYOnScreen(position.y), facing);

    #ifdef DEBUG
    SDL_Rect box;

    //Drawing hurtboxes
    const std::vector<Hurtbox>& hurtboxes = current_animation.getHurtboxes();
    if (state == State::HITSTUN){
        SDL_SetRenderDrawColor(target, 255, 255, 0, 255);
    } else {
        SDL_SetRenderDrawColor(target, 0, 255, 0, 255);
    }
    for (unsigned int i = 0; i < hurtboxes.size(); i++){
        box.w = hurtboxes[i].w;
        box.h = hurtboxes[i].h;
        box.x = cam.getXOnScreen(this->position.x + hurtboxes[i].getRealXPos(facing));
        box.y = cam.getYOnScreen(this->position.y + hurtboxes[i].y);
        SDL_RenderDrawRect(target, &box);
    }

    //Drawing hitboxes
    const std::vector<Hitbox>& hitboxes = current_animation.getHitboxes();
    SDL_SetRenderDrawColor(target, 255, 0, 0, 255);
    for (unsigned int i = 0; i < hitboxes.size(); i++){
        box.w = hitboxes[i].w;
        box.h = hitboxes[i].h;
        box.x = cam.getXOnScreen(this->position.x + hitboxes[i].getRealXPos(facing));
        box.y = cam.getYOnScreen(this->position.y + hitboxes[i].y);
        SDL_RenderDrawRect(target, &box);
    }

    //Drawing position
    int x = getXOnScreen(cam, position.x);
    int y = getYOnScreen(cam, position.y);

    SDL_SetRenderDrawColor(target, 0, 0, 255, 255);
    SDL_Drawing::drawCross(target, cam.getXOnScreen(position.x), cam.getYOnScreen(position.y), 10);

    //Drawing ID
    const Kuribrawl::Vector& frame_size = current_animation.getAnimation()->getFrameSize();
    Kuribrawl::Text::displayTextCentered(std::to_string(id), x, y - frame_size.y - Kuribrawl::Text::getDebugFont()->char_size.h, target);

    #endif
}

void Fighter::writeDebugInfo(AdvancedTextDisplayer& out){
    out 
        << "Speed X : " << speed.x << AdvancedTextDisplayer::force_advance(120)
        << "   Speed Y : " << speed.y << AdvancedTextDisplayer::force_advance(120)
        << " " << Debug::state_to_string(state);
}

/**
 * @brief Makes the Fighter jump.
 *
 */
void Fighter::ground_jump(jumpX x_type, jumpY y_type){
	x_type = (x_type == jumpX::UndecidedX) ? decideJumpXType() : x_type;
	y_type = (y_type == jumpY::UndecidedY) ? decideGroundedJumpYType() : y_type;

    Champion::DefaultAnimation animation_id;

    switch (y_type){
        case jumpY::Full:
            speed.y += model->values.jump_speed;
            break;
        case jumpY::Short:
            speed.y += model->values.short_hop_speed;
            break;
        default:
            break;
    }
    
    switch(x_type){
        case jumpX::Forward:
            if (Kuribrawl::flipValue(speed.x, facing) < model->values.ground_forward_jump_speed){
                speed.x = Kuribrawl::flipValue(model->values.ground_forward_jump_speed, facing);
            }
            animation_id = Champion::DefaultAnimation::JUMP_FORWARD;
            break;
        case jumpX::Backwards:
            if (Kuribrawl::flipValue(-speed.x, facing) < model->values.ground_backward_jump_speed){
                speed.x = Kuribrawl::flipValue(-model->values.ground_backward_jump_speed, facing);
            }
            animation_id = Champion::DefaultAnimation::JUMP_BACKWARD;
            break;
        default:
            animation_id = Champion::DefaultAnimation::JUMP;
            break;
    }

    setState(State::IDLE, Kuribrawl::Side::NEUTRAL, 1, false);
    setAnimation(animation_id);
    grounded = false;
}


//Returns int to make jump_manager able to return its return value directly
//
int Fighter::air_jump(jumpX x_type){
	if (air_jumps > 0) {    
		x_type = (x_type == jumpX::UndecidedX) ? decideJumpXType() : x_type;
	
        Champion::DefaultAnimation animation_id;

	    switch(x_type){
            case jumpX::Forward:
                if (Kuribrawl::flipValue(speed.x, facing) < model->values.air_forward_jump_speed){
                    speed.x = Kuribrawl::flipValue(model->values.air_forward_jump_speed, facing);
                }
                animation_id = Champion::DefaultAnimation::AIR_JUMP_FORWARD;
                break;
            case jumpX::Backwards:
                if (Kuribrawl::flipValue(speed.x, Kuribrawl::oppSide(facing)) < model->values.air_backward_jump_speed){
                    speed.x = Kuribrawl::flipValue(model->values.air_backward_jump_speed, Kuribrawl::oppSide(facing));
                }
                animation_id = Champion::DefaultAnimation::AIR_JUMP_BACKWARD;
                break;
            default:
                animation_id = Champion::DefaultAnimation::AIR_JUMP;
                break;
        }
		
        speed.y = model->values.air_jump_speed;
		air_jumps--;
		setState(Fighter::State::IDLE, Kuribrawl::Side::NEUTRAL, 0, false);
        setAnimation(animation_id);
        
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
/*
bool Fighter::isStateFinished(int duration){
    return (duration == -1) ? current_animation.is_finished() : state_timer >= duration;
}
*/
/*
void Fighter::checkStateDuration(){
    switch (state){
        case State::JUMPSQUAT:
            if (!model->values.jump_squat_duration){
                ground_jump();
            }
            break;
        case State::DASH_START:
            if (!model->values.dash_start_duration){
                setState(State::DASH);
            }
            break;
        case State::DASH_STOP:
            if (!model->values.dash_stop_duration){
                setState(State::IDLE);
            }
            break;
        case State::DASH_TURN:
            if (!model->values.dash_turn_duration){
                setState(State::DASH);
            }
            break;
        case State::LANDING:
            if (!model->values.landing_duration){
                setState(State::IDLE);
            }
            break;

        default:
            break;
    }
}
*/

/**
 * @brief Checks if any change of state should be made based on the current state and the \ref Fighter#state_time "state timer".
 *
 */
void Fighter::updateState(){

    if (paused > 0){
        --paused;
        return;
    }
    ++state_timer;

    switch (state){
        case State::JUMPSQUAT:
            if (current_animation.is_finished()){
                ground_jump(static_cast<jumpX>(state_info & 0b11), (state_info & 0b100) ? jumpY::Short : jumpY::UndecidedY);
            }
            break;
        case State::DASH_START:
            if (current_animation.is_finished()){
                setState(State::DASH);
            }
            break;
        case State::DASH_STOP:
            if (current_animation.is_finished()){
                setState(State::IDLE);
            }
            break;
        case State::DASH_TURN:
            if (current_animation.is_finished()){
                setState(State::DASH);
            }
            break;
        case State::LANDING:
        case State::LANDING_LAG:
            Debug::out << "Fighter " << id << " is in landing state\n" << std::flush;
            if (current_animation.is_finished()){
                Debug::out << ("End Landing ") << id << '\n';
                setState(State::IDLE);
            }
            break;
        case State::HITSTUN:    //State info is the hitstun duration
            if (state_timer >= state_info) {
                setState(State::IDLE, Kuribrawl::Side::NEUTRAL, 2, Champion::DefaultAnimation::AIR_IDLE_AFTER_HIT);
            }
            break;
        case State::ATTACK:
            if (current_animation.is_finished()){
                Debug::out << "Animation end behavior : " << (int)current_animation.getEndBehavior() << '\n';
                switch (current_animation.getEndBehavior()){
                    case GameplayAnimationBehavior::EndingBehavior::CUSTOM:
                        throw KBFatalExplicit("CUSTOM end mode is not supported yet. Shouldn't have been present in a loaded animation anyway, this is weird");
                        break;
                    case GameplayAnimationBehavior::EndingBehavior::HELPLESS:
                        setState(Fighter::State::FREEFALL);
                        break;
                    case GameplayAnimationBehavior::EndingBehavior::IDLE:
                        switch (current_move->end_behavior){
                            case Move::EndBehavior::NORMAL:
                                setState(Fighter::State::IDLE);
                                break;
                            case Move::EndBehavior::FREEFALL:
                                setState(Fighter::State::FREEFALL);
                                break;
                        }
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
/*
void Fighter::checkUpdateAnimation(){
    if (update_anim){
        updateAnimation();
        update_anim = false;
    }
}
*/

void Fighter::trySetAnimFromState(Fighter::State state_){
    const EntityAnimation* anim;
    switch(state_){
        case State::IDLE:
            anim = (grounded) ? 
                model->getDefaultAnimation(Champion::DefaultAnimation::IDLE) : 
                (state_info == 1) ? model->getDefaultAnimation(Champion::DefaultAnimation::JUMP) : model->getDefaultAnimation(Champion::DefaultAnimation::AIR_IDLE);
            
            if (anim)
                setAnimation(anim);
            break;
        case State::HITSTUN:
            break;
        default:
            duration_t state_duration = model->getStateDuration(state_);
            anim = model->getDefaultAnimation((Champion::DefaultAnimation)state_);
            if (anim)
                setAnimation(anim, state_duration);

    }
}


void Fighter::updateAnimation(){
    trySetAnimFromState(state);
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

Game& Fighter::getGame() const{
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

void Fighter::setState(const Fighter::State s, Kuribrawl::Side facing_, int info, bool update_anim_){
    Debug::logFrame(s, *this);
    state = s;
    state_info = info;
    //update_anim = update_anim_;
    if (facing_ != Kuribrawl::Side::NEUTRAL) facing = facing_;

    state_timer = 0;
    paused = 0;
    //checkStateDuration();

    if (update_anim_){
        trySetAnimFromState(s);
    }
}

void Fighter::setState(const Fighter::State s, Kuribrawl::Side facing, int info, Champion::DefaultAnimation anim, double speed){
    setAnimation(anim);
    setAnimation(anim, speed);
    
    setState(s, facing, info, false);
}

void Fighter::setState(const State s, Kuribrawl::Side facing, int info, const EntityAnimation &anim, double speed){
    setAnimation(&anim);
    setAnimation(&anim, speed);
    
    setState(s, facing, info, false);
}

bool Fighter::startMove(const Move& move){
    current_move = &move;
    if (move.animation){
        setAnimation(move.animation);
        return true;
    }
    return false;
}

/**
 * @brief Starts an attack.
 * 
 * Sets the current state to "Attack" and starts a move. 
 * 
 * @param move 
 * @return true if the attack could be started
 * @return false otherwise
 */
bool Fighter::attack(const Move& move){
    if (startMove(move)){
        setState(Fighter::State::ATTACK);
        return true;
    }
    return false;
}

void Fighter::setID(int id){
    this->id = id;
}

std::ostream &operator<<(std::ostream &os, const Fighter &f)
{
    return os << "Fighter <" << f.id << '>';
}
