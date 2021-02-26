#pragma once
#include <string>
#include <map>
#include "util.h"

class Animation;

class Champion {
    public:

    struct ShieldInfo : Kuribrawl::Vector {
        int size;
    };

    struct Values {
        double walk_speed;
        double dash_speed;
        double dash_start_speed;
        double dash_turn_accel;
        double traction; //grounded horizontal deceleration
        double max_air_speed;
        double air_acceleration;
        double air_friction;
        double jump_speed;
        double short_hop_speed;
        double midair_jump_speed;
        double gravity;
        double max_fall_speed;
        double fast_fall_speed;
        int jump_squat_duration;
        int dash_start_duration;
        int dash_stop_duration;
        int dash_turn_duration;
        int landing_duration;
        int guard_start_duration;
        int guard_stop_duration;
        Champion::ShieldInfo shield_info;
        double weight;
    };

    Champion();

    Champion::Values val;

    private:
    std::string name;
    std::string display_name;
    std::map<std::string, Animation> animations;
    std::string current;
};

