#pragma once
#include <string>
#include <memory>
#include <array>
#include <unordered_set>
#include "Util/containers_util.h"
#include "SDL2/SDL.h"
#include "Util/util.h"
#include "Display/AnimationsPool.h"
#include "Move.h"
#include "Display/EntityAnimation.h"
#include "Util/DoubleKeyMap.h"
#include "FighterStates.h"

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
        JUMP_FORWARD,
        JUMP_BACKWARD,
        AIR_JUMP,
        AIR_JUMP_FORWARD,
        AIR_JUMP_BACKWARD,
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

    using StateDurations_t = std::array<double, (size_t)Kuribrawl::FighterState::STATES>;

    Champion(const std::string& name_);
    Champion(std::string&& name_);
    Champion(const std::string_view& name_);
    const std::string& getName();
    const EntityAnimation* getDefaultAnimation(const DefaultAnimation state) const;
    void setDefaultAnimation(const DefaultAnimation state, const EntityAnimation* anim);
    const EntityAnimation* getAnimationTransition(const EntityAnimation&, const EntityAnimation&) const;
    void initDefaultAnimations();
    const EntityAnimation* resolveDefaultAnimation(DefaultAnimation id, std::unordered_set<DefaultAnimation>&);
    void initAnimationTransitionMatrix();

    Move& addMove(const std::string& name);
    Move& addMove(std::string&& name);
    const Move* getMove(const char* name) const;
    const Move* getMove(const std::string& name) const;
    Move& tryMove(const char* name);
    Move& tryMove(std::string&& name);
    const Move* getDefaultMove(DefaultMoves) const;
    double getStateDuration(Kuribrawl::FighterState) const;

    void initDefaultMoves();
    void finalizeMoves();
    void finalize();

    void setDisplayName(const char* display_name);
    const std::string& getDisplayName() const;

    Values values;

    private:
    /**
     * @brief Map associating each DefaultAnimation with the name (=string key) is is supposed to have in the animation map.  
     * Used only once on initialization to construct the ::default_animations array.
     */
    static const std::map<DefaultAnimation, std::string> default_animation_name;
    /** @brief Same as ::default_animation_name for Moves*/
    static const std::map<DefaultMoves, std::string> default_move_name;
    /** @brief Map associating a DefaultAnimation with another DefaultAnimation that can be used instead. */
    static const std::map<DefaultAnimation, DefaultAnimation> default_animations_fallbacks;
    /** @brief Matrix associating a couple of default animations with the name of the animation that should be used as a transition between them.*/
    static const Kuribrawl::DynamicMatrixST<DefaultAnimation, std::string> default_transition_animations_names;

    const std::string name;   ///< Internal identifier of this Champion.
    std::string display_name;   ///< Name that will be displayed for this Champion.
    std::unique_ptr<const EntityAnimation*[]> default_animations; /**< Array associating each \ref Fighter#State "fighter state" to an animation.
                                                    Pointer validity : can be invalidated if an Animation is moved or deleted*/
    std::unique_ptr<const Move*[]> default_moves;
    
    std::map<std::string, Move, std::less<>> moves;

    Kuribrawl::DynamicMatrixST<const EntityAnimation*, const EntityAnimation*> transition_matrix;

    StateDurations_t state_durations;
};

