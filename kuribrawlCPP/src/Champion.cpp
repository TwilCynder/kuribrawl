#include "Champion.h"
#include "Debug.h"

#define DEFAULT_GRAVITY 0.6

Champion::Champion(const std::string& name_):
    name(name_)
{
    this->val.gravity = DEFAULT_GRAVITY;
    this->val.jump_speed = 8;
    this->val.walk_speed = 2.0;
}

Animation* Champion::addAnimation(const std::string& name){
    auto [node, success] = animations.try_emplace(name);

    if (!success) {
        throw KBFatal("Could not create champion");
    }

    Animation& anim = node->second;
    return &anim;
}

Animation* Champion::addAnimation(const std::string& name, SDL_Texture* spritesheet){
    auto [node, success] = animations.try_emplace(name, spritesheet);

    if (!success) {
        throw KBFatal("Could not create champion");
    }

    Animation& anim = node->second;
    return &anim;
}

Animation* Champion::getAnimation(const std::string& name){
    auto it = animations.find(name);
    if (it == animations.end()){
        return NULL;
    }
    return &(it->second);
}

void Champion::initAnimations(){
    
}