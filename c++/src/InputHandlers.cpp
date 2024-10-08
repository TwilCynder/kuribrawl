#include "PlayerFighter.h"
#include "Ground.h"
#include "KBDebug/Debug.h"
#include "KBDebug/DebugMiscGameplay.h"

int PlayerFighter::InputHandler_Jump(RegisteredInput& input){
    if (paused) return InputHandlerResult::POSTPONE;
    return jump_manager(input, jumpY::Full);
}

int PlayerFighter::InputHandler_ShortHop(RegisteredInput& input){
    if (paused) return InputHandlerResult::POSTPONE;
    return jump_manager(input, jumpY::Short);
}


int PlayerFighter::InputHandler_SmashStickSide(RegisteredInput& input){

    Kuribrawl::Side side = (input.input == Input::LEFT) ? Kuribrawl::Side::LEFT : Kuribrawl::Side::RIGHT;

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
        return InputHandlerResult::HANDLED;
    }
    return InputHandlerResult::HANDLED;
}

int PlayerFighter::InputHandler_Attack(RegisteredInput& input){
    using MoveID = Champion::DefaultMoves;
    DirectionIG direction;
    MoveID moveID;
    if (state == State::JUMPSQUAT || state == State::LANDING){
        return InputHandlerResult::DISMISSED;
    }

    direction = Kuribrawl::DirectionToDirectionIG(input.direction, facing);

    Debug::out << "ATTACK INPUT | Stick direction : " << input.direction << " | Relative direction : " << direction << '\n';

    if (!grounded){
        //direction = (input.data & 1) ? Kuribrawl::DirectionToDirectionIG((Direction)(input.data >> 1), facing) : getControlDirection4IG();

        switch (direction){
            case DirectionIG::NONE:
                Debug::log("Nair");
                moveID = MoveID::Nair;
                break;
            case DirectionIG::UP:
                Debug::log("Uair");
                moveID = MoveID::UAir;
                break;
            case DirectionIG::DOWN:
                moveID = MoveID::DAir;
                Debug::log("Dair");
                break;
            case DirectionIG::FORWARD:
                moveID = MoveID::Fair;
                Debug::log("Fair");
                break;
            case DirectionIG::BACK:
                moveID = MoveID::BAir;
                Debug::log("Bair");
                break;
            default:
                break;
        }

        const Move* move = getChampion().getDefaultMove(moveID);
        Debug::out << "Move : " << move << '\n';
        attack(*move);
    }

    return InputHandlerResult::HANDLED;
}

int PlayerFighter::InputHandler_SmashStickDown(RegisteredInput& input){
    if (grounded){
        Debug::log(current_ground);
        if (current_ground && current_ground->isTraversable())
            ground_interaction = GroundInteraction::HARD;
    } else if (speed.y < 0.0 || paused){
        speed.y = - (getChampion().values.fast_fall_speed);
    }

    return InputHandlerResult::HANDLED;
}

int PlayerFighter::InputHandler_SmashStickUp(RegisteredInput& input){
    Debug::log("INPUT HANDLER");
    //Debug::log((void*)input_binding);

    if (input_binding->tap_jump){
        jump_manager(input, jumpY::Full);
    }
    return InputHandlerResult::HANDLED;
}

PlayerFighter::InputHandler PlayerFighter::input_handlers[Input::TOTAL];

void PlayerFighter::initInputHandlers(){
    input_handlers[Input::JUMP] = &PlayerFighter::InputHandler_Jump;
    input_handlers[Input::SHORTHOP] = &PlayerFighter::InputHandler_ShortHop;
    input_handlers[Input::RIGHT] = &PlayerFighter::InputHandler_SmashStickSide;
    input_handlers[Input::LEFT] = &PlayerFighter::InputHandler_SmashStickSide;
    input_handlers[Input::DOWN] = &PlayerFighter::InputHandler_SmashStickDown;
    input_handlers[Input::UP] = &PlayerFighter::InputHandler_SmashStickUp;
    input_handlers[Input::ATTACK] = &PlayerFighter::InputHandler_Attack;
}