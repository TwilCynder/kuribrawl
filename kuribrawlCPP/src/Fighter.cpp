#include "Fighter.h"

Fighter::Fighter(Champion* model_)
{
    Fighter(model_, 0, 0);
}

Fighter::Fighter(Champion* model_, int x_, int y_):
    model(model_)
{
    Animation* idle_anim = model->getAnimation("idle");
    current_animation.setAnimation(idle_anim, 0.05);

    position.x = x_;
    position.y = y_;
}

bool Fighter::is_initialized(){
    return current_animation.is_initialized();
}

const Kuribrawl::Vector* Fighter::getPosition(){
    return &position;
}

CurrentAnimation* Fighter::getCurrentAnimation(){
    return &current_animation;
}

void Fighter::draw(SDL_Renderer* target, int x, int y){
    current_animation.draw(target, x, y);
}

const std::map<Fighter::State, std::string> Fighter::state_default_animation_name = {
    {Fighter::State::IDLE, "idle"},
    {Fighter::State::WALK, "walking"},
    {Fighter::State::WALK_TURN, "walk_turn"},
    {Fighter::State::JUMPSQUAT, "jumpsquat"},
    {Fighter::State::LANDING, "landing"}
}