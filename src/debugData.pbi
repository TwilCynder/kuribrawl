Dim InputNames.s(#INPUTS)
InputNames(#INPUT_Attack) = "Attack"
InputNames(#INPUT_Special) = "Special"
InputNames(#INPUT_Jump) = "Jump"
InputNames(#INPUT_Grab) = "Grab"
InputNames(#INPUT_Guard) = "Guard"
InputNames(#INPUT_ControlStick_RIGHT) = "Stick Right"
InputNames(#INPUT_ControlStick_UP) = "Stick Up"
InputNames(#INPUT_ControlStick_DOWN) = "Stick Down"
InputNames(#INPUT_ControlStick_LEFT) = "Stick Left"
InputNames(#INPUT_ControlStick_SRIGHT) = "Stick Smash Right"
InputNames(#INPUT_ControlStick_SUP) = "Stick Smash Up"
InputNames(#INPUT_ControlStick_SDOWN) = "Stick Smash Down"
InputNames(#INPUT_ControlStick_SLEFT) = "Stick Smash Left"

Dim StateNames.s(#STATES)

StateNames(#STATE_IDLE) = "Idle"
;IDLE_AIR = IDLE && !grounded
StateNames(#STATE_CROUCH) = "Crouch"
StateNames(#STATE_WALK) = "Walk"
StateNames(#STATE_WALK_TURN) = "Walk Turn"
StateNames(#STATE_DASH_START) = "Initial Dash"
StateNames(#STATE_DASH) = "Dash"
StateNames(#STATE_DASH_TURN) = "Dash Turn"
StateNames(#STATE_DASH_STOP) = "Dash Stop"
StateNames(#STATE_JUMPSQUAT) = "Jumpsquat"
StateNames(#STATE_LANDING) = "Landing"
StateNames(#STATE_ATTACK) = "Attack"
;ATTACK_AIR = ATTACK && !grounded
StateNames(#STATE_LANDING_LAG) = "Landing Lag"
StateNames(#STATE_HITSTUN) = "Hitstun"
StateNames(#STATE_TUMBLE) = "Tumble"
StateNames(#STATE_CROUCH) = "Crouch"
StateNames(#STATE_CROUCH_START) = "Crouch Start"
StateNames(#STATE_CROUCH_STOP) = "Crouch Stop"
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 35
; EnableXP