#pragma once
#include <map>
#include <string>
#include "SDL2/SDL_render.h"
#include "Util/stringOperations.h"

/**
 * @brief Class containing any number of Animations, each associated with a name (string identifier)
 *
 */
template <class A>
class AnimationsPool {
    public:
    A& addAnimation(std::string&& name, SDL_Texture* spritesheet);
    A& addAnimation(const char* name, SDL_Texture* spritesheet);
    A& addAnimation(const Kuribrawl::string_view name, SDL_Texture* spritesheet);
    const A* getAnimation(const std::string& name) const;
    const A* getAnimation(const char* name) const;
    const A* getAnimation(const Kuribrawl::string_view& name) const;
    A& tryAnimation(const char* name);
    A& tryAnimation(const Kuribrawl::string_view& sv);
    A& tryAnimation(const std::string& name);
    A& tryAnimation(std::string&& name);

    protected:
    std::map<std::string, A, std::less<>> animations;    ///< Map containing all the Animations of this Champion.
};