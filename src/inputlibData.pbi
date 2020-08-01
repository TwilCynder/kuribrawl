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

;- Controllers

defaultControler.knownController

defaultControler\config\analogStickThreshold = 300
defaultControler\config\analogStickSmashThreshold = 750
defaultControler\config\analogTriggerThreshold = -500

defaultBind.InputBinding
With defaultControler\defaultBinding
  AddElement(\buttons()) : \buttons()\ID = #BUTTON_A : \buttons()\input = #INPUT_Attack
  AddElement(\buttons()) : \buttons()\ID = #BUTTON_B : \buttons()\input = #INPUT_Special
  AddElement(\buttons()) : \buttons()\ID = #BUTTON_X : \buttons()\input = #INPUT_Jump
  AddElement(\buttons()) : \buttons()\ID = #BUTTON_Y : \buttons()\input = #INPUT_Jump
  AddElement(\buttons()) : \buttons()\ID = #BUTTON_Z : \buttons()\input = #INPUT_Grab
  AddElement(\axises()) : \axises()\ID = #AXIS_RIGHT : \axises()\input = #INPUT_Attack
  AddElement(\triggers()) : \triggers()\ID = #AXIS_LEFT : \triggers()\input = #INPUT_Guard
  AddElement(\triggers()) : \triggers()\ID = #AXIS_RIGHT : \triggers()\input = #INPUT_Jump
  \controlStickID = 0
EndWith


gameCube.knownController
gameCube\config\analogStickThreshold = 300
gameCube\config\analogStickSmashThreshold = 750
gameCube\config\analogTriggerThreshold = -500

With gameCube\defaultBinding
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



; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 38
; EnableXP