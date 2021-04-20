#pragma once
#include <map>
#include <string>
#include <memory>
#include "SDL2/SDL.h"
#include "CurrentAnimation.h"
#include "util.h"
#include "InputManager.h"
#include "gameActions.h"

class Port;
class Champion;
class InputManager;

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
        IDLE,   ///< Doing nothing, free.
        GUARD,  ///< Protecting themselves (can only do a limited set of actions, shield is activated)
        GUARD_START,    ///< Started when a Fighter attemps to Guard, automatically transitions into Guard after a few frames.
        GUARD_STOP,     ///< Started when a Fighter left the Guard state in certain ways, transitionns into Idle after a few frames.
        WALK,           ///< Moving slowly on ground.
        WALK_TURN,      ///< Changing direction during Walk.
        DASH,           ///< Moving rapidly on ground.
        DASH_START,     ///< Started when a Fighter attemps to Dash, transitions into Dash after a few frames, the fighter moves (generally faster than during dash).
        DASH_STOP,      ///< Started when a Fighter left the Dash state in certain ways. Transitions into Idle after a few frames, the fighter gradually loses speed.
        DASH_TURN,      ///< Changing direction during Dash.
        JUMPSQUAT,      ///< Started whan a Fighter attemps to Jump, transitions into Idle and boost vertical speed after a few frames.
        LANDING,        ///< Started when a Fighter hits the ground after being previously airborne 
        ATTACK,         ///< Attacking (in the animation of an attack)
        HITSTUN,        ///< Unable to act after getting hit
        TUMBLE,         ///< Started after the hitstun from a strong hit ended, actions are a bit restrcited.
        GUARD_STUN,     ///< Unable to act or to put down shield after getting their shield it. 
        LANDING_LAG,    ///< Started when a Fighter hits the ground after being previously airborne and while the state was Attacking.
        CROUCH,         
        CROUCH_START,
        CROUCH_STOP,
        STATES          ///< Never used, is (because of how enums work) the total number of states.
    };

    Fighter(Champion* model_);
    Fighter(Champion* model_, int x, int y);

    void draw(SDL_Renderer* target);
    //Physics
    void applyPhysics();
    //Inputs
    void updateInputs();
    //States
    void updateState();
    void updateAnimation();

    State getState() const;
    void setState(const State s, int facing = 0, int info = 0, bool update_anim_ = true);
    CurrentAnimation* getCurrentAnimation();
    Kuribrawl::VectorDouble& getPosition();
    void setSpeed(double x, double y);
    bool getGrounded() const;

    void jump(jumpX x_type, jumpY y_type);

    InputManager* getInputManager() const;
    Port* getPort() const;
    void setPort(Port* port);

    bool is_initialized();

    /**
     * @brief Map associating each state with the name of the Animation that has to be used by a Champion as the default Animation for this State.
     */
    static const std::map<State, std::string> state_default_animation_name; 

    private:
    Champion* model;    ///<Champion this Fighter is based on. Pointer validity : can be invalidated if a champion is deleted or moved (should not happen while a Fighter instance exists)

    CurrentAnimation current_animation; ///< CurrentAnimation used to display an Animation of the \ref Fighter#model "model Champion".
    std::string current_animation_name; ///< Never used.

    Kuribrawl::VectorDouble position;   ///< Current position of the Fighter in the Stage the game is playing in.
    Kuribrawl::VectorDouble speed;      ///< Current speed to the Fighter.
    Port* port;         ///<Port controlling this Fighter. Pointer validity : dla merde
    std::unique_ptr<InputManager> input_manager;    ///< InputManager ised to process the input made by the Port.

    State state;        ///< Current State.
    int state_info;     ///< Additional data that can be set when a state is started.
    int state_timer;    ///< Number of frames this the current state was started.
    bool update_anim;   ///< Whether the animation should be updated according to the current state.
    int paused;         ///< If >0, no gameplay property (like speed, position, state timer, etc) as well as the CurrentAnimation will be updated.\ Is decremented each frame. 
    int facing;         ///< 1 if the Fighter is facing left, -1 if they're facing right. 
    bool grounded;      ///< true if the Fighter is on the ground.

    bool isStateFinished(int stateDuration);

    //Inputs
    void checkStickState();

    //Physics
    void groundCollision();
    void groundToAir();
};