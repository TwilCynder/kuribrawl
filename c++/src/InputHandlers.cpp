#include "PlayerFighter.h"
#include "KBDebug/Debug.h"

int PlayerFighter::InputHandler_Jump(RegisteredInput& input){
    if (paused) return InputHandlerResult::POSTPONE;
    return jump_manager(input, jumpY::Full);
}

int PlayerFighter::InputHandler_ShortHop(RegisteredInput& input){
    if (paused) return InputHandlerResult::POSTPONE;
    return jump_manager(input, jumpY::Short);
}


int PlayerFighter::InputHandler_SmashStickSide(RegisteredInput& input){

    int side = (input.input == Input::LEFT) ? -1 : 1;

    if (grounded){
        if (state == Fighter::State::WALK ||
            state == Fighter::State::IDLE ||
            state == Fighter::State::LANDING ||
            (state == Fighter::State::DASH_START && side == -facing))
        {
            setState(Fighter::State::DASH_START, side);
        } else if ((state == Fighter::State::DASH || state == Fighter::State::DASH_STOP) &&
            side == -facing)
        {
            setState(Fighter::State::DASH_TURN, side);
        }
    }

    return InputHandlerResult::HANDLED;
}

int PlayerFighter::InputHandler_Attack(RegisteredInput& input){

    using Move = Champion::DefaultMoves;
    DirectionIG direction;
    Move move;
    if (state == State::JUMPSQUAT || state == State::LANDING){
        return InputHandlerResult::DISMISSED;
    }

    if (!grounded){

        direction = (input.data & 1) ? Kuribrawl::DirectionToDirectionIG((Direction)(input.data >> 1), facing) : getControlDirection4IG();

        switch (direction){
            case DirectionIG::NONE:
                Debug::log("Nair");
                move = Move::Nair;
                break;
            case DirectionIG::UP:
                Debug::log("Uair");
                move = Move::UAir;
                break;
            case DirectionIG::DOWN:
                move = Move::Nair;
                Debug::log("Dair");
                break;
            case DirectionIG::FORWARD:
                move = Move::Nair;
                Debug::log("Fair");
                break;
            case DirectionIG::BACK:
                move = Move::Nair;
                Debug::log("Bair");
                break;
            default:
                break;
        }

        attack(*getChampion().getDefaultMove(move));
    }

    return InputHandlerResult::HANDLED;
}

int PlayerFighter::InputHandler_SmashStickDown(RegisteredInput& input){
    if (!grounded && (speed.y < 0.0 || paused)){
        speed.y = - (getChampion().val.fast_fall_speed);
    }
    return InputHandlerResult::HANDLED;
}

int PlayerFighter::InputHandler_SmashStickUp(RegisteredInput& input){
    if (input_binding->tap_jump){
        jump_manager(input, jumpY::Full);
    }
    return InputHandlerResult::HANDLED;
}

PlayerFighter::InputHandler PlayerFighter::input_handlers[Input::TOTAL];

void PlayerFighter::initInputHandlers(){
    input_handlers[Input::JUMP] = &InputHandler_Jump;
    input_handlers[Input::SHORTHOP] = &InputHandler_ShortHop;
    input_handlers[Input::RIGHT] = &InputHandler_SmashStickSide;
    input_handlers[Input::LEFT] = &InputHandler_SmashStickSide;
    input_handlers[Input::DOWN] = &InputHandler_SmashStickDown;
    input_handlers[Input::UP] = &InputHandler_SmashStickUp;
    input_handlers[Input::ATTACK] = &InputHandler_Attack;
}