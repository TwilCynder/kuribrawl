#pragma once

#include <forward_list>
#include "AnimationsPool.h"

class Animation;
class EntityAnimation;

class AnimationResolver {

    template <class A>
    struct Binding {
        const AnimationPool<A>& pool;
        const std::string animation_name;
        A** target;
    };

    std::forward_list<Binding<Animation>> animations;
    std::forward_list<Binding<EntityAnimation>> entity_animations;

    public:
    template<class A>
    void add(const AnimationsPool<A>& pool, std::string&& name, A** target);
};