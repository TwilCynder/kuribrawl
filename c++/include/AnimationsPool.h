#pragma once
#include <map>
#include "Animation.h"

/**
 * @brief Class containing any number of Animations, each associated with a name (string identifier)
 *
 */
class AnimationsPool {
    public:
    Animation* addAnimation(const std::string& name);
    Animation* addAnimation(const std::string& name, SDL_Texture* spritesheet);
    const Animation* getAnimation(const std::string& name) const;
    Animation& tryAnimation(const std::string& name);
    protected:
    std::map<std::string, Animation> animations;    ///< Map containing all the Animations of this Champion.
};