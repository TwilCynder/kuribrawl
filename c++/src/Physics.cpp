#include <math.h>
#include "Fighter.h"
#include "Champion.h"
#include "KBDebug/Debug.h"
#include "Util/util.h"
#include "Game.h"
#include "Stage.h"

#define MAX_SPEED_PRECISION 0.01 //Speeds below this value will be nullified

void Fighter::applyAirAccel(int direction){
	if (!(Kuribrawl::sign(speed.x) == direction && abs(speed.x) > model->values.max_air_speed)){
		speed.x += model->values.air_acceleration * direction;
		if (speed.x > model->values.max_air_speed)
			speed.x = model->values.max_air_speed;
		else if (speed.x < -model->values.max_air_speed)
			speed.x = -model->values.max_air_speed;
	}
}

/**
 * @brief Called when a Fighter touches the ground while being airborne.
 *
 */
void Fighter::groundCollision(){
    
    if (state == Fighter::State::ATTACK && current_move->landing_lag != -1){
        setAnimation(Champion::DefaultAnimation::LANDING, current_move->landing_lag);
    } else {
        setAnimation(Champion::DefaultAnimation::LANDING);
    }
    setState(Fighter::State::LANDING, 0, 0, false);

    land();
}

/**
 * @brief Called when a Fighter was grounded but is no longer on the ground.
 *
 */
void Fighter::groundToAir(){
    grounded = false;
    updateAnimation();
}

void Fighter::land(){
    grounded = true;
    air_jumps = model->values.air_jumps;
}

/**
 * @brief Apply physics-related mechanics.
 *
 */
void Fighter::applyPhysics(Game& game){
    if (paused) return;

    Kuribrawl::VectorDouble new_pos;

    if (!facing) facing = 1;

    if (abs(speed.x) < MAX_SPEED_PRECISION) speed.x = 0;
    if (abs(speed.y) < MAX_SPEED_PRECISION) speed.y = 0;

    if (speed.y > -model->values.max_fall_speed){
        speed.y -= model->values.gravity;

        if (speed.y < -model->values.max_fall_speed){
            speed.y = -model->values.max_fall_speed;
        } 
    }
/*
      If *fighter\state = #STATE_HITSTUN
        *fighter\physics\v\y - #HITSTUN_GRAVITY
      Else
        *fighter\physics\v\y - #GRAVITY
      EndIf   
      If *fighter\physics\v\y < -*fighter\character\maxFallSpeed
        *fighter\physics\v\y = -*fighter\character\maxFallSpeed
      EndIf 
    EndIf 

*/

    //Application des frictions

    if (grounded) {
        Kuribrawl::substractValue(&speed.x, model->values.traction);
    } else {
        Kuribrawl::substractValue(&speed.x, model->values.air_friction);
    }

    //Applications des vitesses dues aux states

    switch (state) {
        case State::WALK:
            speed.x = model->values.walk_speed * facing;
            break;
        case State::DASH:
            speed.x = model->values.dash_speed * facing;
            break;
        case State::DASH_START:
            speed.x = model->values.dash_start_speed * facing;
            break;
        case State::DASH_TURN:
            speed.x += model->values.dash_turn_accel * facing;
        default:
            break;
    }

    const EntityFrame::FrameMovement& frame_movement = current_animation.getFrameMovement();
    applyFrameMovement(speed.x, frame_movement.x);
    applyFrameMovement(speed.y, frame_movement.y);

    new_pos.x = position.x + speed.x;
    new_pos.y = position.y + speed.y;

    //Calcul des collisions

    const Stage* stage = game.getStage();
    //Maybe test if stage isn't null ? Shouldn't happen tho

    if (new_pos.y <= 0){
        new_pos.y = 0;
        speed.y = 0;
        if (!grounded) groundCollision();
    } else if (grounded){
        groundToAir();
    }

    position.x = new_pos.x;
    position.y = new_pos.y;
}