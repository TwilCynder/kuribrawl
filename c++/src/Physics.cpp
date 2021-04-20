#include <math.h>
#include "Fighter.h"
#include "Champion.h"
#include "Debug.h"
#include "util.h"

#define MAX_SPEED_PRECISION 0.01 //Speeds below this value will be nullified

namespace {
    void land(Fighter* fighter){

    }
    void applyAirAccel(Fighter* fighter, int direction){

    }
    void applyAirDrift(Fighter* fighter, int direction){
        
    }

}

/**
 * @brief Called when a Fighter touches the ground while being airborne.
 * 
 */
void Fighter::groundCollision(){
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