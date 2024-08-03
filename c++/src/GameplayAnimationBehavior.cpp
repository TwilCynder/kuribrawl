#include "GameplayAnimationBehavior.h"
#include "Champion.h"
#include <algorithm>

bool GameplayAnimationBehaviorUnresolved::LandingBehaviorWindowComparator::operator()(const LandingBehaviorWindow & a, const LandingBehaviorWindow & b) const
{
    return a.frame - b.frame;
}

constexpr GameplayAnimationBehaviorUnresolved::LandingBehaviorWindowComparator comp;
/**
 * @brief Finalizes the data, making it suitable for conversion to GameplayAnimationBehavior
 * 
 */
void GameplayAnimationBehaviorUnresolved::finalize() {
    std::sort(landing_behavior.begin(), landing_behavior.end(), comp);
}

/**
 * @brief Construct a new Gameplay Animation Behavior from an Unresolved one.
 * 
 * Assumes the unresolved instance has been finalized (GameplayAnimationBehaviorUnresolved::finalize)
 * 
 * @param base 
 * @param champion 
 */
GameplayAnimationBehavior::GameplayAnimationBehavior(const GameplayAnimationBehaviorUnresolved & base, const Champion & champion):
    end_behavior(base.end_behavior)
{   
    landing_behavior.reserve(base.landing_behavior.size());
    for (auto& window_base : base.landing_behavior){
        LandingBehaviorType type = window_base.behavior.type;
        auto& window = landing_behavior.emplace_back();
        window.frame = window_base.frame;
        window.behavior.type = type;
        switch (type)
        {
        case LandingBehaviorType::NOTHING:
            break;
        case LandingBehaviorType::NORMAL:
            window.behavior.normal.duration = window_base.behavior.normal.duration;
            break;
        case LandingBehaviorType::ANIMATION:
            window.behavior.animation.duration = window_base.behavior.animation.duration;

            const EntityAnimation* anim = champion.getAnimation(window_base.behavior.animation.anim_name);

            if (!anim){
                throw new KBFatalExplicit("In fighter animation behavior configuration : behavior refers to animation %s, which does not exist for champion %s", window_base.behavior.animation.anim_name, champion.getDisplayName());
            }

            window.behavior.animation.anim = anim;
        default:
            break;
        }
    }
}
