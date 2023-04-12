#include "Animation/AnimationResolver.h"

template <class A>
AnimationResolver::Binding<A>::Binding(const AnimationsPool<A>& pool_, const std::string&& animation_name_, A& target_) :
    pool(pool_), animation_name(std::move(animation_name_)), target(target_)
{}

template <>
void AnimationResolver::add(const AnimationsPool<Animation>& pool, const std::string&& name, Animation& target){
    animations.emplace_front(pool, std::move(name), target);
}
template <>
void AnimationResolver::add(const AnimationsPool<EntityAnimation>& pool, const std::string&& name, EntityAnimation& target){
    entity_animations.emplace_front(pool, std::move(name), target);
}