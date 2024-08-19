#include <math.h>
#include "Fighter.h"
#include "Champion.h"
#include "KBDebug/Debug.h"
#include "Util/util.h"
#include "Game.h"
#include "Stage.h"
#include "Ground.h"
#include "Platform.h"

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
    
    Debug::out << "Ground collision from fighter " << this->id << '\n' << std::flush;

    if (state == Fighter::State::ATTACK){

        const EntityAnimation::LandingBehavior* anim_land_behavior = current_animation.getLandingBehavior();

        if (anim_land_behavior){
            Debug::log("=========== LANDING ===========");
            Debug::out << "Landing type : " << anim_land_behavior->type;
            
            switch (anim_land_behavior->type){
                case EntityAnimation::LandingBehaviorType::NORMAL:{
                    setState(State::LANDING_LAG, Kuribrawl::Side::NEUTRAL, 0, false);
                    double landing_duration = anim_land_behavior->normal.duration;
                    if (landing_duration > -1){
                        setAnimation(Champion::DefaultAnimation::LANDING_LAG, landing_duration);
                    } else {
                        setAnimation(Champion::DefaultAnimation::LANDING_LAG);
                    }
                }
                break;

                case EntityAnimation::LandingBehaviorType::ANIMATION:{
                    setState(State::LANDING_LAG, Kuribrawl::Side::NEUTRAL, 0, false);
                    double landing_duration = anim_land_behavior->animation.duration;
                    if (landing_duration > -1){
                        setAnimation(anim_land_behavior->animation.anim, landing_duration);
                    } else {
                        setAnimation(anim_land_behavior->animation.anim);
                    }
                }
                break;
            }
        } else {
            setAnimation(Champion::DefaultAnimation::LANDING);
        }
        
    } else {
        setAnimation(Champion::DefaultAnimation::LANDING);
    }
    setState(Fighter::State::LANDING, Kuribrawl::Side::NEUTRAL, 0, false);

    land();
}

inline void Fighter::onGround_(double height, Kuribrawl::VectorDouble& new_pos, const Ground* ground){
    if (ground_interaction == GroundInteraction::SOFT && (ground && ground->isTraversable())) return;
    speed.y = 0.0;
    new_pos.y = height;
    if (!grounded)
        groundCollision();
    current_ground = ground;
}

inline void Fighter::onGround(double height, Kuribrawl::VectorDouble& new_pos){
    onGround_(height, new_pos, nullptr);
}

inline void Fighter::onGround(double height, Kuribrawl::VectorDouble& new_pos, const Ground& ground){
    onGround_(height, new_pos, &ground);
}

const Platform* Fighter::checkGroundCollision(const Stage* stage, Kuribrawl::VectorDouble& new_pos){
    if (speed.y < 0){
        if (grounded && current_ground && current_ground->isTraversable() && ground_interaction == GroundInteraction::HARD){
            groundToAir(State::IDLE);
            return nullptr;
        }

        if (new_pos.y < 0){
            onGround(0, new_pos);
        } else {
            for (const Platform& plat : stage->getPlatforms()){
                const int half_w = plat.getHalfWidth();
                if (new_pos.x >= plat.getPosition().x - half_w && new_pos.x < plat.getPosition().x + half_w){
                    if (position.y >= plat.getPosition().y && new_pos.y < plat.getPosition().y){
                        onGround(plat.getPosition().y, new_pos, plat);
                        return &plat;
                    }
                }
            }
            //no collision with any platform or ground
            if (grounded){
                groundToAir(State::IDLE);
            }
        }

    }
    return nullptr;
}

/**
 * @brief Called when a Fighter was grounded but is no longer on the ground.
 *
 */
void Fighter::groundToAir(){
    grounded = false;
    current_ground = nullptr;
    updateAnimation();
}

void Fighter::groundToAir(Fighter::State s){
    setState(s);
    groundToAir();
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

    if (!facing) facing = Kuribrawl::Side::RIGHT;

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
            speed.x = Kuribrawl::flipValue(model->values.walk_speed, facing);
            break;
        case State::DASH:
            speed.x = Kuribrawl::flipValue(model->values.dash_speed, facing);
            break;
        case State::DASH_START:
            speed.x = Kuribrawl::flipValue(model->values.dash_start_speed, facing);
            break;
        case State::DASH_TURN:
            speed.x += Kuribrawl::flipValue(model->values.dash_turn_accel, facing);
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
    //Maybe test if stage isn't null ? Shouldn't happen tho (called only if !!game.running, et game.running => game.getStage() != nullptr)

    if (grounded && new_pos.y > position.y){
        groundToAir();
    } else {
        checkGroundCollision(stage, new_pos);
    }

    ground_interaction = GroundInteraction::NONE;

    position.x = new_pos.x;
    position.y = new_pos.y;
}