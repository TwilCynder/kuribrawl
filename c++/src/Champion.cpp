#include "Champion.h"
#include "Debug.h"
#include "Animation.h"

#define DEFAULT_GRAVITY 0.6

Champion::Champion(const std::string& name_):
    name(name_),
    state_animations(std::make_unique<Animation*[]>((int)Fighter::State::STATES))
{
    this->val.gravity = DEFAULT_GRAVITY;
    this->val.jump_speed = 8;
    this->val.walk_speed = 2.0;
    this->val.traction = 0.05;
}

const std::string& Champion::getName(){
    return name;
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

Animation& Champion::tryAnimation(const std::string& name){
    auto [node, success] = animations.try_emplace(name);
    return node->second;
}

Animation* Champion::getStateAnimation(const Fighter::State state) const {
    return state_animations[(int)state];
}

void Champion::initAnimations(){
    Animation* anim;
    for (auto const& [state, name] : Fighter::state_default_animation_name){
        if (anim = getAnimation(name)){
            state_animations[(int)state] = anim;
        }
    }
}