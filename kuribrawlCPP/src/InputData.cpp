#include "ControllersData.h"
#include "inputs.h"
#include "Debug.h"
#include "SDL2/SDL_gamecontroller.h"

void ControllersData::initControllersData(){
    ControllerType& c = addController("GC");

    c.control_stick = 0;
    c.setControllerVals(10000, -16000, 25000);

    c.default_binding->buttons[0] = Input::ATTACK;
    c.default_binding->buttons[1] = Input::SPECIAL;
    c.default_binding->buttons[2] = Input::SHORTHOP;
    c.default_binding->buttons[3] = Input::JUMP;
    c.default_binding->tilt_stick = 1;


    SDL_GameControllerAddMapping("0300000034120000adbe000000000000,vJoy Device,a:b0,b:b1,back:b15,dpdown:b6,dpleft:b7,dpright:b8,dpup:b5,guide:b16,leftshoulder:b9,leftstick:b13,lefttrigger:b11,leftx:a0,lefty:a1,rightshoulder:b10,rightstick:b14,righttrigger:b12,rightx:a3,righty:a4,start:b4,x:b2,y:b3,platform:Windows");
}