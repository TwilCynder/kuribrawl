#include <math.h>
#include "Fighter.h"
#include "Champion.h"
#include "Debug.h"
#include "util.h"

#define MAX_SPEED_PRECISION 0.01 //Speeds below this value will be nullified

void Fighter::applyAirAccel(int direction){
	if (!(sign(speed.x) == direction && abs(speed.x) > model->val.max_air_speed)){
		speed.x += model->val.air_acceleration * direction;
		if (speed.x > model->val.max_air_speed)
			speed.x = model->val.max_air_speed;
		else if (speed.x < -model->val.max_air_speed)
			speed.x = -model->val.max_air_speed;
	}
}

/**
 * @brief Called when a Fighter touches the ground while being airborne.
 *
 */
void Fighter::groundCollision(){
    setState(Fighter::State::LANDING);
    grounded = true;
}

/**
 * @brief Called when a Fighter was grounded but is no longer on the ground.
 *
 */
void Fighter::groundToAir(){
    grounded = false;
}

/**
 * @brief Apply physics-related mechanics.
 *
 */
void Fighter::applyPhysics(){

    if (paused) return;

    Kuribrawl::VectorDouble new_pos;

    if (!facing) facing = 1;

    if (abs(speed.x) < MAX_SPEED_PRECISION) speed.x = 0;
    if (abs(speed.y) < MAX_SPEED_PRECISION) speed.y = 0;

    speed.y -= model->val.gravity;

    //Application des frictions

    if (grounded) Kuribrawl::substractValue(&speed.x, model->val.traction);

    //Applications des vitesses dues aux states

    switch (state) {
        case State::WALK:
            speed.x = model->val.walk_speed * facing;
            break;
        case State::DASH:
            speed.x = model->val.dash_speed * facing;
            break;
        case State::DASH_START:
            speed.x = model->val.dash_start_speed * facing;
            break;
        case State::DASH_TURN:
            speed.x += model->val.dash_turn_accel * facing;
        default:
            break;
    }

    new_pos.x = position.x + speed.x;
    new_pos.y = position.y + speed.y;

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