#include "stdint.h"
#include "Champion.h"

/**
 * @todo Check if we actually do this
 * 
 */

/**
 * @brief Contains all the fixed (numerical) values of a Character (speeds, weight, lags ...)
 *
 */
struct Values {
    double walk_speed;
    double dash_speed;
    double dash_start_speed;
    double dash_turn_accel;
    double dash_stop_deceleration;
    double traction; //grounded horizontal deceleration
    double max_air_speed;
    double air_acceleration;
    double air_friction;
    double jump_speed;
    double short_hop_speed;
    double air_jump_speed;
    double ground_forward_jump_speed;
    double ground_backward_jump_speed;
    double air_forward_jump_speed;
    double air_backward_jump_speed;
    double gravity;
    double max_fall_speed;
    double fast_fall_speed;
    double weight;
    int8_t jump_squat_duration;
    int8_t dash_start_duration;
    int8_t dash_stop_duration;
    int8_t dash_turn_duration;
    int8_t landing_duration;
    int8_t guard_start_duration;
    int8_t guard_stop_duration;
    Champion::ShieldInfo shield_info;
    Uint8 air_jumps;

    Values();

    enum class Indexes {
        WALK_SPEED,
        DASH_SPEED,
        DASH_START_SPEED,
        DASH_TURN_ACCEL,
        DASH_STOP_DECELERATION,
        TRACTION,
        MAX_AIR_SPEED,
        AIR_ACCELERATION,
        AIR_FRICTION,
        JUMP_SPEED,
        SHORT_HOP_SPEED,
        AIR_JUMP_SPEED,
        GROUND_FORWARD_JUMP_SPEED,
        AIR_FORWARD_JUMP_SPEED,
        GROUND_BACKWARD_JUMP_SPEED,
        AIR_BACKWARD_JUMP_SPEED,
        TOTAL
    };

    const std::array<size_t, (size_t)Indexes::TOTAL> offsets = {
        offsetof(Values, walk_speed),
        offsetof(Values, dash_speed),
        offsetof(Values, dash_start_speed),
        offsetof(Values, dash_turn_accel),
        offsetof(Values, dash_stop_deceleration),
        offsetof(Values, traction),
        offsetof(Values, max_air_speed),
        offsetof(Values, air_acceleration),
    };
};