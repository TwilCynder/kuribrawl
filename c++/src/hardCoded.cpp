#include "hardCoded.h"
#include "GameData.h"
#include "CollisionBoxes.h"
#include "inputs.h"
#include "Debug.h"
#include "SDL2/SDL_gamecontroller.h"
#include "SDL2/SDL_image.h"

/**
 * @brief Initializes all hard-coded informations related to Controllers, updating and populating a given ControllersData in consequence.
 *
 * @param data the ControllersData object that will contain the hard-coded data.
 */
void HardCoded::initControllersData(ControllersData& data){
    ControllerType& c = data.addController("GC");

    c.control_stick = 0;
    c.setControllerVals(10000, -16000, 20000);

    c.default_binding->buttons[0] = Input::ATTACK;
    c.default_binding->buttons[1] = Input::SPECIAL;
    c.default_binding->buttons[2] = Input::SHORTHOP;
    c.default_binding->buttons[3] = Input::JUMP;
    c.default_binding->tilt_stick = 1;
    SDL_GameControllerAddMapping("0300000034120000adbe000000000000,vJoy Device,a:b0,b:b1,back:b15,dpdown:b6,dpleft:b7,dpright:b8,dpup:b5,guide:b16,leftshoulder:b9,leftstick:b13,lefttrigger:b11,leftx:a0,lefty:a1,rightshoulder:b10,rightstick:b14,righttrigger:b12,rightx:a3,righty:a4,start:b4,x:b2,y:b3,platform:Windows");
}

/**
 * @brief Initializes all hard-coded informations related to Controllers, updating and populating the \ref App#game_data "game_data member" of a given App in consequence.
 * Uses the renderer of the App to load images.
 * \todo Make it take a GameData as paramater when this function stops loading images.
 * @param data the ControllersData object that will contain the hard-coded data.
 */
void HardCoded::initGameData(App* app){
    GameData& data = app->gameData();
    Champion* acid = data.addChampion("Acid");
    EntityAnimation* anim;

	anim = acid->addAnimation("idle", app->LoadTexture("../res/idle_strip4.png"));
	anim->initFrames(4);
    anim->setBaseSpeed(0.05);
    anim->addHurtbox(0, -20, 0, 40, 50);
    anim->addHurtbox(1, -20, 0, 40, 52);
    anim->addHurtbox(2, -20, 0, 40, 54);
    anim->addHurtbox(3, -20, 0, 40, 52);

    anim = acid->addAnimation("walking", app->LoadTexture("../res/walk_strip6.png"));
    anim->initFrames(6);
    anim->setBaseSpeed(0.15);

    anim = acid->addAnimation("jumpsquat", app->LoadTexture("../res/jumpstart.png"));
    anim->initFrames(1);
    anim->setBaseSpeed(-1);

    anim = acid->addAnimation("dash", app->LoadTexture("../res/dash_strip4.png"));
    anim->initFrames(4);
    anim->setBaseSpeed(0.18);

    anim = acid->addAnimation("dash_start", app->LoadTexture("../res/dashstart_strip2.png"));
    anim->initFrames(2);
    anim->setBaseSpeed(8);

    anim = acid->addAnimation("dash_stop", app->LoadTexture("../res/dashstop_strip2.png"));
    anim->initFrames(2);
    anim->setBaseSpeed(1);

    anim = acid->addAnimation("dash_turn", app->LoadTexture("../res/dashturn_strip3.png"));
    anim->initFrames(3);
    anim->setBaseSpeed(14);

    anim = acid->addAnimation("air_idle", app->LoadTexture("../res/air_idle.png"));
    anim->initFrames(1);
    anim->setBaseSpeed(-1);

    anim = acid->addAnimation("jump", app->LoadTexture("../res/jump_strip5.png"));
    anim->initFrames(5);
    anim->setBaseSpeed(0.5);
    anim->setNextAnimation(acid->getAnimation("air_idle"));

    anim = acid->addAnimation("landing", app->LoadTexture("../res/land_strip2.png"));
    anim->initFrames(2);
    anim->setBaseSpeed(5);

    acid->initAnimations();
}