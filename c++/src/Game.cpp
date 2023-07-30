#include <iostream>
#include <vector>
#include "Game.h"
#include "Util/util.h"
#include "Champion.h"
#include "Stage.h"
#include "StageModel.h"
#include "KBDebug/Debug.h"
#include "CollisionBoxes.h"
#include "KBDebug/DebugTime.h"
#include "GameConfiguration.h"
#include "PlayerConfiguration.h"
#include "Util/Text/TextureFont.h"
#include "Util/Text/TextDisplayer.h"
#include "defs.h"
#include "GameConstants.h"

using namespace std;

/**
 * @brief 
 * 
 *
 */
Game::Game():
    frame(0),
    original_config(nullptr)
{
}

Game::Game(GameConfiguration& config) :
    Game()
{
    Debug::log("Constructed Game using config object");
    applyConfig(config);
    original_config = std::make_shared<GameConfiguration>(config);
}

Game::Game(GameConfSPtr& config_sptr) : 
    Game()
{
    Debug::log("Constructed Game using existing sptr to config object");
    original_config = config_sptr;
    applyConfig(*config_sptr);
}

Game::~Game(){
    Debug::log("Game destroyed with facts and logic.");
}

void Game::applyConfig(GameConfiguration& config){
    //Appliquer règles

    int w = SCREEN_WIDTH;
    int step = w / config.players.size();
    int x = -((config.players.size() - 1) * step / 2.0);

    const StageModel* stageModel = config.stage;
    if (!stageModel) throw KBFatal("Starting game will null stage");
    stage = std::make_unique<Stage>(*stageModel);

    for (const StageBackgroundElement& background_element : stage->getBackgroundElements()){
        graphics.add(background_element, background_element.getModel().depth);
    }

    for (const Platform& platform : stage->getPlatforms()){
        graphics.add(platform);
    }

    #ifdef DEBUG
    int i = 0;
    #endif
    for (PlayerConfiguration& player : config.players){
        Fighter* f;
        if (player.port != nullptr){
            f = addFighter(&player.champion, x, 100, *player.port);
        } else {
            f = addFighter(&player.champion, x, 100);
        }
        x += step;

        #ifdef DEBUG
        f->setID(i);
        i++;
        #endif
    }
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

    onFighterAdded(*it);

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
    Debug::log("added fighter");
    Fighteriterator it = fighters.before_begin();
    it = fighters.emplace_after(it, *this, model, x, y);

    onFighterAdded(*it);

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

    onFighterAdded(*it);

    return &(*it);
}

void Game::onFighterAdded(const Fighter& fighter){
    fighters++;
    graphics.add(fighter);
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
    /*Fighteriterator it;
    for (it = fighters.begin(); it != fighters.end(); ++it){
        it->draw(target);
    }*/

    updateCameraPosition(target);
    
    graphics.draw(target, camera);
}

/**
 * @brief
 * 
 */
void Game::updateInputsState(){
    Fighteriterator it;
    for (it = fighters.begin(); it != fighters.end(); ++it){
        it->updateInputsStates();
    }
}

/**
 * @brief
 * 
 */
void Game::resolveInputs(){
    Fighteriterator it;
    for (it = fighters.begin(); it != fighters.end(); ++it){
        it->resolveInputs();
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
        it->checkUpdateAnimation();
    }
}

/**
 * @brief \ref Fighter#applyPhysics "Processes physics mechanics " on every Fighter.
 * This includes modifying speed based on states, applying speed, and detecting collisions and terrain interaction.  
 * May only be called if `running` is true
 */
void Game::applyPhysics(){

    Fighteriterator it;
    for (it = fighters.begin(); it != fighters.end(); ++it){
        it->applyPhysics(*this);
    }
}

/**
 * @brief \ref AnimationPlayer#advance "Advances" the AnimationPlayer of every Fighter.
 * 
 */
void Game::advanceAnimations(){
    Fighteriterator it;
    for (it = fighters.begin(); it != fighters.end(); ++it){
        it->advanceAnimation();
    }
}

inline bool testRectCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2){
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 > y2 - h2 &&
            y1 - h1 < y2
    );
}

void hit(Fighter& attacker, Fighter& defender, const Hitbox& hitbox, const Hurtbox& hurtbox){
    if (attacker.hitFighter(defender, hitbox, hurtbox)){
        defender.getHit(attacker, hitbox, hurtbox);
    }
}

void Game::hitDetection(){

    Fighteriterator attacker, defender;
    HurtboxVector::const_iterator hurtbox;
    HitboxVector::const_iterator hitbox;

    for (attacker = fighters.begin(); attacker != fighters.end(); ++attacker){

        if (!attacker->is_initialized())
            throw KBFatal("Uninitialized fighter (at hit detection stage), ID : %d", attacker->id);


        const Kuribrawl::VectorDouble& attackerPos = attacker->getPosition();
        const HitboxVector & hitbox_vector  = attacker->getCurrentHitboxes();
        for (defender = fighters.begin(); defender != fighters.end(); ++defender){
            
            if (attacker == defender) continue;
            const Kuribrawl::VectorDouble& defenderPos = defender->getPosition();
            const HurtboxVector& hurtbox_vector = defender->getCurrentHurtboxes();
            for (hitbox = hitbox_vector.begin(); hitbox != hitbox_vector.end(); ++hitbox){

                for (hurtbox = hurtbox_vector.begin(); hurtbox != hurtbox_vector.end(); ++hurtbox){
                    
                    if (testRectCollision(attackerPos.x + hitbox->getRealXPos(attacker->getFacing()), attackerPos.y + hitbox->y, hitbox->w, hitbox->h,
                        defenderPos.x + hurtbox->getRealXPos(attacker->getFacing()), defenderPos.y + hurtbox->y, hurtbox->w, hurtbox->h))
                    {
                        hit(*attacker, *defender, *hitbox, *hurtbox);
                    }
                }
            }
        }
    }
}

void Game::updateCameraPosition(SDL_Renderer* target){


    if (fighters.empty()) return;

    /*
    //average position of the fighters, aka the target for the camera pos
    ArithVec2<double> avgPos = {0, 0}; 
    for (auto& fighter : fighters){
        avgPos += fighter.getPosition();
    }
    avgPos /= fighters.size();
    
    //we want the actual average pos on the center of the screen
    avgPos.y -= SCREEN_HEIGHT / 2;
    */

    Kuribrawl::Rect<double> fighters_box;
    {
        auto fighter_it = fighters.begin();
        const auto& pos = fighter_it->getPosition();
        
        fighters_box.left = pos.x;
        fighters_box.top = pos.y;
        fighters_box.right = pos.x;
        fighters_box.bottom = pos.y;

        for (++fighter_it; fighter_it != fighters.end(); ++fighter_it){
            const auto& pos = fighter_it->getPosition();
            if (pos.x < fighters_box.left) fighters_box.left = pos.x;
            if (pos.y > fighters_box.top) fighters_box.top = pos.y;
            if (pos.x > fighters_box.right) fighters_box.right = pos.x;
            if (pos.y < fighters_box.bottom) fighters_box.bottom = pos.y;
        }
    }

    Kuribrawl::Vector mid_pos = {
        (fighters_box.right - fighters_box.left) / 2 + fighters_box.left,
        (fighters_box.top - fighters_box.bottom) / 2 + fighters_box.bottom
    };


    SDL_SetRenderDrawColor(target, 255, 255, 0, 255);
    SDL_Rect fbrect {camera.getXOnScreen(fighters_box.left), camera.getYOnScreen(fighters_box.top), fighters_box.right - fighters_box.left, fighters_box.top - fighters_box.bottom};
    SDL_RenderDrawRect(target, &fbrect);

    SDL_SetRenderDrawColor(target, 255, 255, 0, 255);

    SDL_RenderDrawLine(target, camera.getXOnScreen(mid_pos.x - 10), camera.getYOnScreen(mid_pos.y), camera.getXOnScreen(mid_pos.x + 10), camera.getYOnScreen(mid_pos.y));
    SDL_RenderDrawLine(target, camera.getXOnScreen(mid_pos.x), camera.getYOnScreen(mid_pos.y) - 10, camera.getXOnScreen(mid_pos.x), camera.getYOnScreen(mid_pos.y) + 10);

    SDL_SetRenderDrawColor(target, 255, 255, 255, 255);
    SDL_RenderDrawLine(target, SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2 + 10, SCREEN_HEIGHT / 2);
    SDL_RenderDrawLine(target, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 10, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 10);

    /*
    Ideas : 
    - Si on reste longtemps dans la no-move zone mais toujours du même côté (donc longtemps nettement d'un côté de l'écran), on ignore la nomove zone et on bouge la caméra
    - Hors no-move zone (fighter box trop grande), ne bouger la cam que si les fighters se rapprochent ?
    */
    if (fighters_box.right - fighters_box.left < game_constants_calc.camera_nomove_area_size.x){
        Kuribrawl::Rectangle current_nomove_area = {
            camera.position.x + game_constants_calc.camera_nomove_area.left,
            camera.position.y + game_constants_calc.camera_nomove_area.top,
            camera.position.x + game_constants_calc.camera_nomove_area.right,
            camera.position.y + game_constants_calc.camera_nomove_area.bottom
        };

        SDL_SetRenderDrawColor(target, 255, 0, 255, 255);
        SDL_Rect fbrect {camera.getXOnScreen(current_nomove_area.left), camera.getYOnScreen(current_nomove_area.top), current_nomove_area.right - current_nomove_area.left, current_nomove_area.top - current_nomove_area.bottom};
        SDL_RenderDrawRect(target, &fbrect);

        int camera_movement_x = 0;
        if      (fighters_box.left < current_nomove_area.left) camera_movement_x = fighters_box.left - current_nomove_area.left;
        else if (fighters_box.right > current_nomove_area.right) camera_movement_x = fighters_box.right - current_nomove_area.right;

        if (abs(camera_movement_x) > game_constants.camera_max_speed){
            camera.position.x += game_constants.camera_max_speed * Kuribrawl::sign(camera_movement_x);
        } else {
            camera.position.x += camera_movement_x;
        }

        /*
        int camera_movement_x = 0;
        if      (fighters_box.left < current_nomove_area.left) camera_movement_x = fighters_box.left - current_nomove_area.left;
        else if (fighters_box.right > current_nomove_area.right) camera_movement_x = fighters_box.right - current_nomove_area.right;

        if (abs(camera_movement_x) > game_constants.camera_max_speed){
            camera.position.x += game_constants.camera_max_speed * Kuribrawl::sign(camera_movement_x);
        } else {
            camera.position.x += camera_movement_x;
        }
        */

    } else {


        int camera_movement_x = mid_pos.x - camera.position.x;
        if (camera_movement_x){
            if (abs(camera_movement_x) <= game_constants.camera_max_speed){
                camera.position.x = mid_pos.x;
            } else {
                camera.position.x += game_constants.camera_max_speed * Kuribrawl::sign(camera_movement_x);
            }
        }
    }   

    /* //Pour si jamais on repart sur un truc qui traite pas x et y séparément
    ArithVec2<double> distance = avgPos - camera.position;
    double distSquared = distance.normSquare(); //manipulating squared distances to go faster

    if (distSquared < camera_max_speed_squared){
        camera.position = avgPos;
    } else {

    }*/

    

}

void Game::drawDebugInfo(TextureFont& font, SDL_Renderer* target){
    Fighteriterator it;
    int y = 20;
    int i = 1;

    for (it = fighters.begin(); it != fighters.end(); ++it){
        AdvancedTextDisplayer td = AdvancedTextDisplayer(20, y, font) << "Fighter " << i << " : " ;
        it->writeDebugInfo(td);
        y += 20;
        i++;
    }

    SDL_Rect box = {20, y, 100, 100};
    for (PlayerFighter& fighter : fighters){
        fighter.drawDebugInfo(target, box);
        box.x += box.w;
        box.w = 100;
        box.h = 100;
    }
}

/**
 * @brief Does everything a Game needs to do during one iteration of the main loop
 * 
 */

void Game::step(SDL_Renderer* render_target){
    updateInputsState();
    updateStates();
    resolveInputs();
    updateAnimations();
    //From this point on every animation change must be done manually   
    applyPhysics();
    hitDetection();
    draw(render_target);
    advanceAnimations();
    frame++;
}

Game::Result::Result() : 
    completed(false),
    original_config(nullptr)
{}

Game::Result::Result(bool completed_, GameConfSPtr&& config_) : 
    completed(completed_),
    original_config(config_)
{}

Game::Result Game::stop(){
    Result res(true, std::move(original_config));
    return res;
}

GameConfiguration* Game::getOriginalConfiguration() const {
    return original_config.get();
}

const GameConfSPtr& Game::getOriginalConfigurationSharedPtr() const {
    return original_config;
}

int Game::getFrame(){
    return frame;
}

/**
 * @brief Returns the current Stage.  
 * Cannot return null if all invariant are respected. 
 * 
 * @return const Stage* 
 */
const Stage* Game::getStage() const {
    return stage.get();
}