#include "hardCoded.h"
#include "GameData.h"
#include "CollisionBoxes.h"
#include "inputs.h"
#include "Debug.h"
#include "SDL2/SDL_gamecontroller.h"
#include "SDL2/SDL_image.h"

#include "controllerElements.h"

/**
 * @brief Initializes all hard-coded informations related to Controllers, updating and populating a given ControllersData in consequence.
 *
 * @param data the ControllersData object that will contain the hard-coded data.
 */
void HardCoded::initControllersData(ControllersData& data){
    const char * mapping_string;
    ControllerType* c;

    c = &data.addController("GC");

    c->control_stick = 0;
    c->secondary_stick = 1;
    c->setControllerVals(10000, 18000, 20000);
    c->default_binding->buttons[0] = Input::ATTACK;
    c->default_binding->buttons[1] = Input::SPECIAL;
    c->default_binding->buttons[2] = Input::SHORTHOP;
    c->default_binding->buttons[3] = Input::JUMP;
    c->default_binding->buttons[8] = Input::UP;
    c->default_binding->buttons[9] = Input::DOWN;
    c->default_binding->buttons[10] = Input::LEFT;
    c->default_binding->buttons[11] = Input::RIGHT;
    c->default_binding->triggers[TRIGGER_RIGHT] = Input::JUMP;
    c->default_binding->second_stick = Input::ATTACK;
    c->default_binding->dpadAnalogValue = 32000;
    c->default_binding->direction_control_mode = Binding::DirectionControlMode::BOTH;


    mapping_string = "0300000034120000adbe000000000000,Gamecube Controller vJoy,platform:Windows,a:b0,b:b1,y:b2,x:b3,start:b7,rightshoulder:b4,dpup:b8,dpleft:b10,dpdown:b9,dpright:b11,leftx:a0,lefty:a1,rightx:a3,righty:a4,lefttrigger:a2,righttrigger:a5";
    SDL_GameControllerAddMapping(mapping_string);
    data.mapSDLMappingToControllerType(mapping_string, c);

    c = &data.addController("PS4");
    c->control_stick = 0;
    c->secondary_stick = 1;
    c->setControllerVals(10000, 18000, 20000);
    c->default_binding->buttons[0] = Input::ATTACK;
    c->default_binding->buttons[1] = Input::SPECIAL;
    c->default_binding->buttons[2] = Input::SHORTHOP;
    c->default_binding->buttons[3] = Input::JUMP;
    c->default_binding->buttons[11] = Input::UP;
    c->default_binding->buttons[12] = Input::DOWN;
    c->default_binding->buttons[13] = Input::LEFT;
    c->default_binding->buttons[14] = Input::RIGHT;
    c->default_binding->triggers[TRIGGER_RIGHT] = Input::JUMP;
    c->default_binding->dpadAnalogValue = 32000;
    c->default_binding->direction_control_mode = Binding::DirectionControlMode::BOTH;
    mapping_string = "030000004c050000cc09000000000000,Sony DualShock 4 V2,a:b1,b:b2,back:b13,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,dpup:h0.1,guide:b12,leftshoulder:b4,leftstick:b10,lefttrigger:a3,leftx:a0,lefty:a1,rightshoulder:b5,rightstick:b11,righttrigger:a4,rightx:a2,righty:a5,start:b9,x:b0,y:b3,platform:Mac OS X,";
    SDL_GameControllerAddMapping(mapping_string);
    data.mapSDLMappingToControllerType(mapping_string, c);
}

/**
 * @brief Initializes all hard-coded informations related to Controllers, updating and populating the \ref App#game_data "game_data member" of a given App in consequence.
 * Uses the renderer of the App to load images.
 * \todo Make it take a GameData as paramater when this function stops loading images.
 * @param data the ControllersData object that will contain the hard-coded data.
 */
void HardCoded::initGameData(App* app){
    GameData& data = app->gameData();
    Champion* acid = &data.tryChampion("acid");
    EntityAnimation* anim;

    /*anim = acid->addAnimation("walking", app->LoadTexture("../res/old/walk_strip6.png"));
    anim->initFrames(6);
    anim->setBaseSpeed(0.15);

    anim = acid->addAnimation("dash", app->LoadTexture("../res/old/dash_strip4.png"));
    anim->initFrames(4);
    anim->setBaseSpeed(0.18);

    anim = acid->addAnimation("dash_start", app->LoadTexture("../res/old/dashstart_strip2.png"));
    anim->initFrames(2);
    anim->setBaseSpeed(8);

    anim = acid->addAnimation("dash_stop", app->LoadTexture("../res/old/dashstop_strip2.png"));
    anim->initFrames(2);
    anim->setBaseSpeed(5);

    anim = acid->addAnimation("dash_turn", app->LoadTexture("../res/old/dashturn_strip3.png"));
    anim->initFrames(3);
    anim->setBaseSpeed(14);

    anim = acid->addAnimation("air_idle", app->LoadTexture("../res/old/air_idle.png"));
    anim->initFrames(1);
    anim->setBaseSpeed(-1);

    anim = acid->addAnimation("jump", app->LoadTexture("../res/old/jump_strip5.png"));
    anim->initFrames(5);
    anim->setBaseSpeed(0.5);

    anim = acid->addAnimation("landing", app->LoadTexture("../res/old/land_strip2.png"));
    anim->initFrames(2);
    anim->setBaseSpeed(5);*/
}