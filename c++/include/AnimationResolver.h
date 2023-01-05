#pragma once

#include <forward_list>
#include "AnimationsPool.h"

class Animation;
class EntityAnimation;

class AnimationResolver {

    public:
    template <class A>
    struct Binding {
        const AnimationsPool<A>& pool;
        const std::string animation_name;
        A&;

        Binding(const AnimationsPool<A>& pool, const std::string&& animation_name, A& target, int nbFrames);
    };

    template<class A>
    void add(const AnimationsPool<A>& pool, const std::string&& name, A& target);

    private:
    std::forward_list<Binding<Animation>> animations;
    std::forward_list<Binding<EntityAnimation>> entity_animations;
};