#include "ChampionValues.h"

#define DEFAULT_GRAVITY 0.6

/**
 * @brief Construct a new Champion::Values object
 * Uses Acid Rainbows as default values.
 */
Values::Values() :
    walk_speed(4.5), dash_speed(8.0), dash_start_speed(10.0), dash_turn_accel(1.5), dash_stop_deceleration(0.5), traction(0.7),
    max_air_speed(5.0), air_acceleration(0.3), air_friction(0), jump_speed(16), short_hop_speed(8), air_jump_speed(6), 
    ground_forward_jump_speed(3),ground_backward_jump_speed(3),air_forward_jump_speed(3),air_backward_jump_speed(3),
    gravity(DEFAULT_GRAVITY), max_fall_speed(0), fast_fall_speed(0),
    jump_squat_duration(5), dash_start_duration(-1), dash_stop_duration(-1), dash_turn_duration(-1), landing_duration(-1), guard_start_duration(0), guard_stop_duration(0), air_jumps(1)
{
    
}