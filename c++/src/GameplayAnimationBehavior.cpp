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
void GameplayAnimationBehaviorUnresolved::finalize()
{
    landing_behavior.sort(comp);
}

GameplayAnimationBehaviorUnresolved::LandingBehaviorWindow &GameplayAnimationBehaviorUnresolved::add_landing_window(frame_index_t frame)
{
    return landing_behavior.emplace_back(frame);
}

void GameplayAnimationBehaviorUnresolved::add_landing_window_normal(frame_index_t frame, duration_t duration)
{
    auto& window = add_landing_window(frame);
    window.behavior.type = LandingBehaviorType::NORMAL;
    window.behavior.normal.duration = duration;
}

void GameplayAnimationBehaviorUnresolved::add_landing_window_animation(frame_index_t frame, std::string && anim_name, duration_t duration)
{
    auto& window = add_landing_window(frame);
    window.behavior.type = LandingBehaviorType::ANIMATION;
    window.behavior.normal.duration = duration;
    new (&window.behavior.animation.anim_name) std::string(std::move(anim_name));
}

void GameplayAnimationBehaviorUnresolved::add_landing_window_nothing(frame_index_t frame)
{
    auto& window = add_landing_window(frame);
    window.behavior.type = LandingBehaviorType::NOTHING;
}

/**
 * @brief Construct a new Gameplay Animation Behavior from an Unresolved one.
 * 
 * Assumes the unresolved instance has been finalized (GameplayAnimationBehaviorUnresolved::finalize)
 * 
 * @param base 
 * @param champion 
 */
GameplayAnimationBehavior::GameplayAnimationBehavior(const GameplayAnimationBehaviorUnresolved & base, const AnimationsPool<EntityAnimation> & anim_pool)
{   
    setFromUnresolved(base, anim_pool);
}

void GameplayAnimationBehavior::setFromUnresolved(const GameplayAnimationBehaviorUnresolved & base, const AnimationsPool<EntityAnimation> & anim_pool)
{
    end_behavior = base.end_behavior;

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
        case LandingBehaviorType::ANIMATION:{
            Debug::out << "-> Animation name : " << window_base.behavior.animation.anim_name << '\n';

            window.behavior.animation.duration = window_base.behavior.animation.duration;

            const EntityAnimation* anim = anim_pool.getAnimation(window_base.behavior.animation.anim_name);

            if (!anim){
                throw KBFatalExplicit("In fighter animation behavior configuration : behavior refers to animation %s, which does not exist for the given animation pool", window_base.behavior.animation.anim_name.c_str());
            }

            window.behavior.animation.anim = anim;
            break;
        }
        default:
            break;
        }
    }
}

GameplayAnimationBehavior::EndingBehavior GameplayAnimationBehavior::getEndBehavior() const
{
    return end_behavior;
}

GameplayAnimationBehavior::LandingBehaviorWindows GameplayAnimationBehavior::getLandingBehavior() const
{
    return landing_behavior;
}

GameplayAnimationBehaviorUnresolved::LandingBehavior::LandingBehavior()
{}

/*
GameplayAnimationBehaviorUnresolved::LandingBehavior::LandingBehavior(LandingBehaviorType t_):
    type(t_)
{}

GameplayAnimationBehaviorUnresolved::LandingBehavior::LandingBehavior(duration_t duration):
    type(LandingBehaviorType::NORMAL),
    normal{duration}
{}

GameplayAnimationBehaviorUnresolved::LandingBehavior::LandingBehavior(std::string &&anim_name, duration_t duration):
    type(LandingBehaviorType::ANIMATION),
    animation{std::move(anim_name), duration}
{}
*/

GameplayAnimationBehaviorUnresolved::LandingBehavior::~LandingBehavior(){
    if (type == LandingBehaviorType::ANIMATION){
        animation.anim_name.~basic_string();
    }
}

GameplayAnimationBehaviorUnresolved::LandingBehaviorWindow::LandingBehaviorWindow(frame_index_t frame_):
    frame(frame_)
{}
