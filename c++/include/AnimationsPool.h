#pragma once
#include <map>
#include <string>
#include "SDL2/SDL_render.h"

/**
 * @brief Class containing any number of Animations, each associated with a name (string identifier)
 *
 */
template <class A>
class AnimationsPool {
    public:
    A* addAnimation(const std::string& name);
    A* addAnimation(const char* name, SDL_Texture* spritesheet);

    const A* getAnimation(const std::string& name) const;
    const A* getAnimation(const char* name) const;
    A& tryAnimation(const char* name);
    protected:
    std::map<std::string, A, std::less<>> animations;    ///< Map containing all the Animations of this Champion.
};