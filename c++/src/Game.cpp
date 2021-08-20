#include <iostream>
#include "Game.h"
#include "util.h"
#include "CurrentAnimation.h"
#include "Champion.h"
#include "Debug.h"

using namespace std;

/**
 * @brief Construct a new Game object
 * 
 */
Game::Game():
    frame(0),
    slowness(1),
    since_last_update(1)
{
}

/**
 * @brief Construct a new Game object
 * 
 */
Game::Game(Uint8 slowness_):
    frame(0),
    slowness(slowness_),
    since_last_update(slowness_)
{
}

/**
 * @brief Adds a new Fighter to the Game.
 * Constructs the Fighter in-place, at position (0,0)
 * @param model the Champion the new Fighter will be based on.
 * @return Fighter* the Fighter created (and added).
 */
PlayerFighter* Game::addFighter(Champion* model){
    Fighteriterator it = fighters.begin();
    it = fighters.emplace_after(it, *this, model);
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
PlayerFighter* Game::addFighter(Champion* model, int x, int y){
    Fighteriterator it = fighters.before_begin();
    it = fighters.emplace_after(it, *this, model, x, y);
    return &(*it);
}

/**
 * @brief Adds a new Fighter to the Game.
 * Constructs the Fighter in-place.
 * @param model the Champion the new Fighter will be based on.
 * @param x the x position of the new Fighter.
 * @param y the y position of the new Fighter
 * @param port the port being used to control this PlayerFighter.
 * @return Fighter* the Fighter created (and added).
 */
PlayerFighter* Game::addFighter(Champion* model, int x, int y, Port& port){
    Fighteriterator it = fighters.before_begin();
    it = fighters.emplace_after(it, *this, model, x, y, port);
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
    Fighteriterator it;
    for (it = fighters.begin(); it != fighters.end(); ++it){
        it->draw(target);
    }
}

/**
 * @brief \ref Fighter#updateInputs "Updates the inputs" of every Fighter.
 * 
 */
void Game::updateInputs(){
    Fighteriterator it;
    for (it = fighters.begin(); it != fighters.end(); ++it){
        it->updateInputs();
    }
}

/**
 * @brief \ref Fighter#updateState "Updates the state" of every Fighter.
 * 
 */
void Game::updateStates(){
    Fighteriterator it;
    for (it = fighters.begin(); it != fighters.end(); ++it){
        it->updateState();
    }
}

/**
 * @brief \ref Fighter#updateAnimation "Updates the Animation" of every Fighter.
 * 
 */
void Game::updateAnimations(){
    Fighteriterator it;
    for (it = fighters.begin(); it != fighters.end(); ++it){
        it->updateAnimation();
    }
}

/**
 * @brief \ref Fighter#applyPhysics "Processes physics mechanics " on every Fighter.
 * This includes modifying speed based on states, applying speed, and detecting collisions and terrain interaction.
 */
void Game::applyPhysics(){
    Fighteriterator it;
    for (it = fighters.begin(); it != fighters.end(); ++it){
        it->applyPhysics(slowness);
    }
}

/**
 * @brief \ref CurrentAnimation#advance "Advances" the CurrentAnimation of every Fighter.
 * 
 */
void Game::advanceAnimations(){
    Fighteriterator it;
    CurrentAnimation* anim;
    for (it = fighters.begin(); it != fighters.end(); ++it){
        anim = it->getCurrentAnimation();
        if (anim->is_initialized()){
            anim->advance(slowness);
        }
    }
}

/**
 * @brief Does everything a Game needs to do during one iteration of the main loop
 * 
 */

void Game::step(SDL_Renderer* render_target){
    updateStates();
    updateInputs();
    applyPhysics();
    updateAnimations();
    draw(render_target);
    advanceAnimations();
    frame++;
    since_last_update++;
    if (since_last_update > slowness){
        since_last_update = slowness;
    }
}

int Game::getFrame(){
    return frame;
}