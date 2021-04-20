#pragma once
#include <string>
#include <memory>
#include "SDL2/SDL.h"
#include "util.h"
#include "Fighter.h"
#include "AnimationsPool.h"

/**
 * @brief A character of the game.
 * Contain purely static information about this character ; does not care about what happens in-game, which is the job of Fighter.
 */

class Champion : public AnimationsPool {
    public:

    /**
     * @brief Base position and size of this Charater's shield.
     * 
     */
    struct ShieldInfo : Kuribrawl::Vector {
        int size;
    };

    /**
     * @brief Contains all the fixed (numerical) values of a Character (speeds, weight, lags ...)
     * 
     */
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
        double air_jump_speed;
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

        Values();
    };

    Champion(const std::string& name_);
    const std::string& getName();
    Animation* getStateAnimation(const Fighter::State state) const;
    void initAnimations(void);

    Champion::Values val;

    private:
    std::string name;   ///< Internal identifier of this Champion.
    std::string display_name;   ///< Name that will be displayed for this Champion.  
    std::unique_ptr<Animation*[]> state_animations; /**< Array associating each \ref Fighter#State "fighter state" to an animation. 
                                                    Pointer validity : can be invalidated if an Animation is moved or deleted*/
};

