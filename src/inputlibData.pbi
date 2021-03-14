Dim defaultInputDurability(#INPUTS)
defaultInputDurability(#INPUT_Special) = 3
defaultInputDurability(#INPUT_Attack) = 3
defaultInputDurability(#INPUT_Jump) = 3
defaultInputDurability(#INPUT_Grab) = 3
defaultInputDurability(#INPUT_Guard) = 3
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

;- Controllers

gameCube.knownController

gameCube\config\analogStickThreshold = 300
gameCube\config\analogStickSmashThreshold = 750
gameCube\config\analogTriggerThreshold = -500

With gameCube\defaultBinding
  AddElement(\buttons()) : \buttons()\ID = #GCBUTTON_A : \buttons()\input = #INPUT_Attack
  AddElement(\buttons()) : \buttons()\ID = #GCBUTTON_B : \buttons()\input = #INPUT_Special
  AddElement(\buttons()) : \buttons()\ID = #GCBUTTON_X : \buttons()\input = #INPUT_Jump
  AddElement(\buttons()) : \buttons()\ID = #GCBUTTON_Y : \buttons()\input = #INPUT_Jump
  AddElement(\buttons()) : \buttons()\ID = #GCBUTTON_Z : \buttons()\input = #INPUT_Grab
  AddElement(\axises()) : \axises()\ID = #AXIS_RIGHT : \axises()\input = #INPUT_Attack
  AddElement(\triggers()) : \triggers()\ID = #AXIS_LEFT : \triggers()\input = #INPUT_Guard
  AddElement(\triggers()) : \triggers()\ID = #AXIS_RIGHT : \triggers()\input = #INPUT_Jump
  
EndWith
gameCube\controlStickID = 0
gameCube\secondaryStickID = #AXIS_RIGHT

With gameCube\menuBinding
  AddElement(\buttons()) : \buttons()\ID = #GCBUTTON_A : \buttons()\input = #BUTTON_A
  AddElement(\buttons()) : \buttons()\ID = #GCBUTTON_B : \buttons()\input = #BUTTON_B
  AddElement(\buttons()) : \buttons()\ID = #GCBUTTON_X : \buttons()\input = #BUTTON_X
  AddElement(\buttons()) : \buttons()\ID = #GCBUTTON_Y : \buttons()\input = #BUTTON_Y
  AddElement(\buttons()) : \buttons()\ID = #GCBUTTON_Z : \buttons()\input = #BUTTON_RB
  AddElement(\buttons()) : \buttons()\ID = #GCBUTTON_START : \buttons()\input = #BUTTON_START
EndWith
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 24
; EnableXP