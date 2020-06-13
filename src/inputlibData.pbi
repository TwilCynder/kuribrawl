Dim InputNames.s(#INPUTS)
InputNames(#INPUT_Attack) = "Attack"
InputNames(#INPUT_Special) = "Special"
InputNames(#INPUT_Jump) = "Jump"
InputNames(#INPUT_Grab) = "Grab"
InputNames(#INPUT_Guard) = "Guard"
InputNames(#INPUT_ControlStick_RIGHT) = "Control Stick Right"
InputNames(#INPUT_ControlStick_UP) = "Control Stick Up"
InputNames(#INPUT_ControlStick_DOWN) = "Control Stick Down"
InputNames(#INPUT_ControlStick_LEFT) = "Control Stick Left"
InputNames(#INPUT_ControlStick_SRIGHT) = "Control Stick Smash Right"
InputNames(#INPUT_ControlStick_SUP) = "Control Stick Smash Up"
InputNames(#INPUT_ControlStick_SDOWN) = "Control Stick Smash Down"
InputNames(#INPUT_ControlStick_SLEFT) = "Control Stick Smash Left"


defaultBind.InputBinding
With defaultBind
  AddElement(\buttons()) : \buttons()\ID = #BUTTON_A : \buttons()\input = #INPUT_Attack
  AddElement(\buttons()) : \buttons()\ID = #BUTTON_B : \buttons()\input = #INPUT_Special
  AddElement(\buttons()) : \buttons()\ID = #BUTTON_X : \buttons()\input = #INPUT_Jump
  AddElement(\buttons()) : \buttons()\ID = #BUTTON_Y : \buttons()\input = #INPUT_Jump
  AddElement(\buttons()) : \buttons()\ID = #BUTTON_Z : \buttons()\input = #INPUT_Grab
  AddElement(\axises()) : \axises()\ID = #AXIS_RIGHT : \axises()\input = #INPUT_Attack
  AddElement(\triggers()) : \triggers()\ID = #AXIS_LEFT : \triggers()\input = #INPUT_Guard
  AddElement(\triggers()) : \triggers()\ID = #AXIS_RIGHT : \triggers()\input = #INPUT_Guard
  \controlStickID = 0
EndWith

Dim defaultInputDurability(#INPUTS)
defaultInputDurability(#INPUT_Special) = 0
defaultInputDurability(#INPUT_Attack) = 0
defaultInputDurability(#INPUT_Jump) = 0
defaultInputDurability(#INPUT_Grab) = 0
defaultInputDurability(#INPUT_Guard) = 0
defaultInputDurability(#INPUT_ControlStick_SRIGHT) = 5
defaultInputDurability(#INPUT_ControlStick_SDOWN) = 5
defaultInputDurability(#INPUT_ControlStick_SLEFT) = 5
defaultInputDurability(#INPUT_ControlStick_SUP) = 5
defaultInputDurability(#INPUT_ControlStick_RIGHT) = 0
defaultInputDurability(#INPUT_ControlStick_DOWN) = 0
defaultInputDurability(#INPUT_ControlStick_LEFT) = 0
defaultInputDurability(#INPUT_ControlStick_UP) = 0

Prototype inputManager(*port.Port, *info.inputData)

Dim *inputManagers(#INPUTS)
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 44
; EnableXP