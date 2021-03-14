#include <iostream>
#include "Game.h"
#include "util.h"
#include "CurrentAnimation.h"

using namespace std;

Game::Game(){

}

Fighter* Game::addFighter(Champion* model){
    std::forward_list<Fighter>::iterator it = fighters.begin();
    it = fighters.emplace_after(it, model);
    return &(*it);
}

Fighter* Game::addFighter(Champion* model, int x, int y){
    std::forward_list<Fighter>::iterator it = fighters.before_begin();
    it = fighters.emplace_after(it, model, x, y);
    return &(*it);
}

bool Game::is_running(){
    return true;  
}

void Game::draw(SDL_Renderer* target){
    std::forward_list<Fighter>::iterator it;
    const Kuribrawl::Vector* pos;
    for (it = fighters.begin(); it != fighters.end(); ++it){
        pos = it->getPosition();
        it->draw(target, pos->x, pos->y);
    }
}

void Game::advanceAnimations(){
    std::forward_list<Fighter>::iterator it;
    CurrentAnimation* anim;
    for (it = fighters.begin(); it != fighters.end(); ++it){
        anim = it->getCurrentAnimation();
        if (anim->is_initialized()){
            anim->advance();
        }
    }
}