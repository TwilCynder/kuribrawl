#pragma once
#include <string>
#include <map>
#include <memory>
#include "SDL2/SDL.h"
#include "util.h"
#include "Animation.h"
#include "Fighter.h"

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

    Champion(const std::string& name_);
    const std::string& getName();
    Animation* addAnimation(const std::string& name);
    Animation* addAnimation(const std::string& name, SDL_Texture* spritesheet);
    Animation* getAnimation(const std::string& name);
    Animation& tryAnimation(const std::string& name);
    Animation* getStateAnimation(const Fighter::State state) const;
    void initAnimations(void);

    Champion::Values val;

    private:
    std::string name;
    std::string display_name;
    std::map<std::string, Animation> animations;
    std::unique_ptr<Animation*[]> state_animations;  //Pointer validity : can be invalidated if an Animation is moved or deleted
    std::string current;
};
