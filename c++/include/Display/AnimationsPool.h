#pragma once
#include <map>
#include <string>
#include "SDL2/SDL_render.h"
#include "Util/stringOperations.h"
#include "Display/Animation.h"

/**
 * @brief Class containing any number of Animations, each associated with a name (string identifier)
 *
 */
template <class A>
//requires std::is_base_of_v<AnimationBase, A>
class AnimationsPool {

    protected:
    using AnimationMap = std::map<std::string, A, std::less<>>;
    AnimationMap animations;    ///< Map containing all the Animations of this Champion.

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

    using Iterator = AnimationMap::const_iterator;
    Iterator begin() const;
    Iterator end() const;

};