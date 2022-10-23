#pragma once
#include <string>
#include <memory>
#include "SDL2/SDL.h"
#include "Util/util.h"
#include "AnimationsPool.h"
#include "Move.h"
#include "EntityAnimation.h"

/**
 * @brief A character of the game.
 * Contain purely static information about this character ; does not care about what happens in-game, which is the job of Fighter.
 */

class Fighter;

class Champion : public AnimationsPool<EntityAnimation> {
    public:

    /**
     * @brief Animations that are associated with a specific situation.
     */
    enum class DefaultAnimation {
        #include "states.enum"
        AIR_IDLE,
        JUMP,
        AIR_JUMP,
        AIR_HITSTUN_TO_IDLE,
        AIR_IDLE_AFTER_HIT,
        TOTAL
    };

    enum class DefaultMoves {
        Jab,
        Ftilt,
        UTilt,
        DTilt,
        FSmash,
        USmash,
        DSmash,
        Nair,
        Fair,
        BAir,
        UAir,
        DAir,
        NSpecial,
        SSpecial,
        USpecial,
        DSpecial,
        TOTAL
    };

    /**
     * @brief Base position and size of a Charater's shield.
     *
     */
    struct ShieldInfo : Kuribrawl::Vec2<int8_t> {
        Uint8 size;
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
    };

    Champion(const std::string& name_);
    const std::string& getName();
    const EntityAnimation* getDefaultAnimation(const DefaultAnimation state) const;
    void setDefaultAnimation(const DefaultAnimation state, const EntityAnimation* anim);
    void initDefaultAnimations();
    void initDefaultMoves();
    void finalizeMoves();

    Move& addMove(const std::string& name);
    const Move* getMove(const char* name) const;
    const Move* getMove(const std::string& name) const;
    Move& tryMove(const char* name);
    const Move* getDefaultMove(DefaultMoves) const;

    void setDisplayName(const char* display_name);
    const std::string& getDisplayName() const;

    Values val;

    private:
    /**
     * @brief Map associating each DefaultAnimation with the name (=string key) is is supposed to have in the animation map.
     */
    static const std::map<DefaultAnimation, std::string> default_animation_name;
    static const std::map<DefaultMoves, std::string> default_move_name;

    const std::string name;   ///< Internal identifier of this Champion.
    std::string display_name;   ///< Name that will be displayed for this Champion.
    std::unique_ptr<const EntityAnimation*[]> default_animations; /**< Array associating each \ref Fighter#State "fighter state" to an animation.
                                                    Pointer validity : can be invalidated if an Animation is moved or deleted*/
    std::unique_ptr<const Move*[]> default_moves;
    
    std::map<std::string, Move> moves;
};

