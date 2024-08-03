#pragma once

#include <vector>
#include <string>

class EntityAnimation;
class Champion;
class GameplayAnimationBehaviorUnresolved;

/**
 * @brief Describes how an animation should impact the behavior of a Fighter ; that is, what happens when this animation is being played and some events occur.  
 * 
 * These properties as mostly intended to be changed for animations that are used by actual *moves* / attacks ; animations that are used by built-in states *should* keep the default values, but if not, it might just lead to rather unconventional behaviors for the fighter that uses them, but not actual issues.  
 */
class GameplayAnimationBehavior {

    public:

    /**
     * @brief What happens when the animation ends.   
     */
    enum class EndingBehavior {
        IDLE,   ///< The fighter becomes actionable (goes to the IDLE state)
        HELPLESS,   ///< The fighter is put in the HELPLESS state
        CUSTOM  ///< A lua function is called
    };

    /**
     * @brief What type of action happens when a fighter lands while in this animation
     */
    enum class LandingBehaviorType {
        NORMAL,     ///< The fighter goes into the default landing animation
        ANIMATION,  ///< The fighter goes into a specific animation
        NOTHING     ///< Nothing happens : the animation continues
    };

    /**
     * @brief What happens when a fighter lands while in this animation
     */
    struct LandingBehavior {
        LandingBehaviorType type = LandingBehaviorType::NORMAL;
        union {
            /**
             * @brief Data specific to normal landing behavior
             */
            struct {
                int duration; ///< Duration of the landing animation ; a duration of -1 indicates the default duration of the animation
            } normal;

            struct {
                const EntityAnimation* anim;  ///< The animation to transition to
                int duration; ///< Duration of the specified animation ; a duration of -1 indicates the default duration of the animation
            } animation;
        };
    };

    GameplayAnimationBehavior() = default;
    GameplayAnimationBehavior(const GameplayAnimationBehaviorUnresolved&, const Champion&);

    protected:

    /**
     * @brief A behavior associated with a frame ID
     */
    struct LandingBehaviorWindow {
        LandingBehavior behavior;
        int frame = 0;
    };

    EndingBehavior end_behavior = EndingBehavior::IDLE; ///< See ::EndingBehavior
    std::vector<LandingBehaviorWindow> landing_behavior; ///< The different landing behaviors that take effect at different windows. Each behavior is associated with the frame it takes effect at. 
};

/**
 * @brief Contains the same information as a GameplayAnimationBehavior, in a different (less optimal) state. 
 * 
 * The way this class is organized makes it better fit for collecting the data from a DataFile ; 
 * it does not contain the same *data*, but all the information it contains is enough to fully initialize any GameplayAnimationBehavior.
 * 
 * The data might be in a state that is not suitable for GameplayAnimationBehavior intialization, until finalize() is called
 */
struct GameplayAnimationBehaviorUnresolved {
    using EndingBehavior = GameplayAnimationBehavior::EndingBehavior;
    using LandingBehaviorType = GameplayAnimationBehavior::LandingBehaviorType;

        struct LandingBehavior {
        LandingBehaviorType type = LandingBehaviorType::NORMAL;
        union {
            /**
             * @brief Data specific to normal landing behavior
             */
            struct {
                int duration; ///< Duration of the landing animation ; a duration of -1 indicates the default duration of the animation
            } normal;

            struct {
                std::string anim_name;
                int duration; ///< Duration of the specified animation ; a duration of -1 indicates the default duration of the animation
            } animation;
        };
    };

    struct LandingBehaviorWindow {
        LandingBehavior behavior;
        int frame = 0;
    };

    EndingBehavior end_behavior = EndingBehavior::IDLE; ///< See ::EndingBehavior
    std::vector<LandingBehaviorWindow> landing_behavior; ///< The different landing behaviors that take effect at different windows. Each behavior is associated with the frame it takes effect at. 

    class LandingBehaviorWindowComparator {
        bool operator()(const LandingBehaviorWindow&, const LandingBehaviorWindow&) const;
    };

    void finalize();
};