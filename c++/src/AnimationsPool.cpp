#include "AnimationsPool.h"
#include "KBDebug/Debug.h"
#include "Animation.h"
#include "EntityAnimation.h"

/**
 * @brief Adds an Animation.
 * Uses in-place construction.
 * @param name the name that will be used as the map key.
 * @return Animation* a pointer to the created Animation.
 */
template <class A>
A* AnimationsPool<A>::addAnimation(const std::string& name){
    auto [node, success] = animations.try_emplace(name);

    if (!success) {
        throw KBFatal("Could not create champion");
    }

    A& anim = node->second;
    return &anim;
}

/**
 * @brief Adds an animation with its spritesheet.
 * Uses in-place construction.
 * @param name the name that will be used as the map key.
 * @param spritesheet the \ref Animation#spritesheet "spritesheet" of this Animation.
 * @return Animation* A pointer to the created Animation.
 */

template<class A>
A* AnimationsPool<A>::addAnimation(const char* name, SDL_Texture* spritesheet){
    if (!spritesheet)
        throw KBFatal("Tried to add animation with null spritesheet");

    auto [node, success] = animations.try_emplace(name, spritesheet);

    if (!success) {
        throw KBFatal("Could not create champion");
    }

    A& anim = node->second;
    return &anim;
}

/**
 * @brief Returns an Animation of this Champion.
 *
 * @param name the internal name (which is also the map key) of the wanted Animation.
 * @return Animation* a pointer to the Animation if the specified name is an existing key, NULL otherwise.
 */
template<class A>
const A* AnimationsPool<A>::getAnimation(const std::string& name) const{
    auto it = animations.find(name);
    if (it == animations.end()){
        return NULL;
    }
    return &(it->second);
}

/**
 * @brief Returns an Animation of this Champion.
 *
 * @param name the internal name (which is also the map key) of the wanted Animation.
 * @return Animation* a pointer to the Animation if the specified name is an existing key, NULL otherwise.
 */
template<class A>
const A* AnimationsPool<A>::getAnimation(const char* name) const{
    auto it = animations.find(name);
    if (it == animations.end()){
        return NULL;
    }
    return &(it->second);
}

/**
 * @brief Returns an Animation, or create it if it doesn't exist yet.
 * Unlike getAnimation, always returns a valid Animation. If the Animation didn't exist, it is \ref Animation::Animation() "default constructed", so it won't have any frames or spritesheet.
 * @param name the name
 * @return Animation&
 */
template<class A>
A& AnimationsPool<A>::tryAnimation(const char* name){
    auto [node, success] = animations.try_emplace(name);
    return node->second;
}

template class AnimationsPool<Animation>;
template class AnimationsPool<EntityAnimation>;