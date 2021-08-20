#pragma once
#include <map>
#include <string>
#include <memory>
#include "SDL2/SDL.h"
#include "CurrentAnimation.h"
#include "util.h"
#include "InputManager.h"
#include "gameActions.h"

class Champion;
class Game;

/**
 * @brief An in-game character ("instance" of a Champion).
 * A Fighter is an in-game entity that can be player-controlled, and bases its animation, properties, and overall gameplay on a Champion.
 */
class Fighter {
    public:

    /**
     * @brief The situation of a Fighter, definind the actions they can take and the behavior of the Fighter as an entity.
     * A Fighter can have only one State at a time.
     * A state being "started" means that the current state of a Fighter was set to this state.
     * @note States don't care about whether a Fighter is on the ground or not, certain states can very well be used no matter that ; what defines if a Fighter is grounded or not is
     * its \ref Fighter#grounded "grounded" attribute, which is updated each frame (based of course on terrain collision)
     */
    enum class State {
        #include "states.enum"
        STATES          ///< Never used, is (because of how enums work) the total number of states.
    };

    Fighter(Game& game, Champion* model_);
    Fighter(Game& game, Champion* model_, int x, int y);

    void draw(SDL_Renderer* target);
    //Physics
    void applyPhysics();
    void applyPhysics(Uint8 slowness);
    //States
    void updateState();
    void updateState(Uint8 slowness);
    void updateAnimation();

    Game& getGame();
    const Champion& getChampion();
    State getState() const;
    void setState(const State s, int facing = 0, int info = 0, bool update_anim_ = true);
    CurrentAnimation* getCurrentAnimation();
    Kuribrawl::VectorDouble& getPosition();
    void setSpeed(double x, double y);
    bool getGrounded() const;
    int getFacing() const;

    void jump(jumpX x_type, jumpY y_type);

    bool is_initialized();

    protected:
    State state;        ///< Current State.
    int state_info;     ///< Additional data that can be set when a state is started.
    float state_timer;  ///< Number of frames this the current state was started.
    bool update_anim;   ///< Whether the animation should be updated according to the current state.
    int paused;         ///< If >0, no gameplay property (like speed, position, state timer, etc) as well as the CurrentAnimation will be updated.\ Is decremented each frame.
    int facing;         ///< 1 if the Fighter is facing left, -1 if they're facing right.
    bool grounded;      ///< true if the Fighter is on the ground.
    Kuribrawl::VectorDouble position;   ///< Current position of the Fighter in the Stage the game is playing in.
    Kuribrawl::VectorDouble speed;      ///< Current speed to the Fighter.

    void applyAirAccel(int direction);

    private:
    const Champion* const model;    /**<Champion this Fighter is based on. Pointer validity : can be invalidated if a champion is deleted or moved (should not happen while a Fighter instance exists)*/
    Game& game;               /**<Game this Fighter belongs to. Pointer validity : can be invalidated if a game is moved or deleted (should not happen during the lifetime of a Fighter)*/

    CurrentAnimation current_animation; ///< CurrentAnimation used to display an Animation of the \ref Fighter#model "model Champion".
    std::string current_animation_name; ///< Never used.

    bool isStateFinished(int stateDuration);
	virtual jumpY decideGroundedJumpYType() const = 0;

    //Physics
    void groundCollision();
    void groundToAir();
	void land();
};