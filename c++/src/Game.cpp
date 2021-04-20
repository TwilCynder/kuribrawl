#include <iostream>
#include "Game.h"
#include "util.h"
#include "CurrentAnimation.h"
#include "Champion.h"

using namespace std;

/**
 * @brief Construct a new Game object
 * 
 */
Game::Game()
{
}

/**
 * @brief Adds a new Fighter to the Game.
 * Constructs the Fighter in-place, at position (0,0)
 * @param model the Champion the new Fighter will be based on.
 * @return Fighter* the Fighter created (and added).
 */
Fighter* Game::addFighter(Champion* model){
    std::forward_list<Fighter>::iterator it = fighters.begin();
    it = fighters.emplace_after(it, model);
    return &(*it);
}

/**
 * @brief Adds a new Fighter to the Game.
 * Constructs the Fighter in-place.
 * @param model the Champion the new Fighter will be based on.
 * @param x the x position of the new Fighter.
 * @param y the y position of the new Fighter
 * @return Fighter* the Fighter created (and added).
 */
Fighter* Game::addFighter(Champion* model, int x, int y){
    std::forward_list<Fighter>::iterator it = fighters.before_begin();
    it = fighters.emplace_after(it, model, x, y);
    return &(*it);
}

/**
 * @brief Returns whether the Game is actually running
 * @return bool
 */
bool Game::is_running(){
    return true;  
}

/**
 * @brief Displays the Game.
 * Displays the Stage and every entity present in it.
 * @param target the renderer the Game will be displayed on.
 */
void Game::draw(SDL_Renderer* target){
    std::forward_list<Fighter>::iterator it;
    for (it = fighters.begin(); it != fighters.end(); ++it){
        it->draw(target);
    }
}

/**
 * @brief \ref Fighter#updateInputs "Updates the inputs" of every Fighter.
 * 
 */
void Game::updateInputs(){
    std::forward_list<Fighter>::iterator it;
    for (it = fighters.begin(); it != fighters.end(); ++it){
        it->updateInputs();
    }
}

/**
 * @brief \ref Fighter#updateState "Updates the state" of every Fighter.
 * 
 */
void Game::updateStates(){
    std::forward_list<Fighter>::iterator it;
    for (it = fighters.begin(); it != fighters.end(); ++it){
        it->updateState();
    }
}

/**
 * @brief \ref Fighter#updateAnimation "Updates the Animation" of every Fighter.
 * 
 */
void Game::updateAnimations(){
    std::forward_list<Fighter>::iterator it;
    for (it = fighters.begin(); it != fighters.end(); ++it){
        it->updateAnimation();
    }
}

/**
 * @brief \ref Fighter#applyPhysics "Processes physics mechanics " on every Fighter.
 * This includes modifying speed based on states, applying speed, and detecting collisions and terrain interaction.
 */
void Game::applyPhysics(){
    std::forward_list<Fighter>::iterator it;
    for (it = fighters.begin(); it != fighters.end(); ++it){
        it->applyPhysics();
    }
}

/**
 * @brief \ref CurrentAnimation#advance "Advances" the CurrentAnimation of every Fighter.
 * 
 */
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