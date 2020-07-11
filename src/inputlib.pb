XIncludeFile "inputlibConstants.pbi"

Structure InputConfig
  analogStickThreshold.l
  analogStickSmashThreshold.l
  analogTriggerThreshold.l
EndStructure

Macro stickTreshold : defaultControler\config\analogStickThreshold : EndMacro
Macro stickSmashTreshold : defaultControler\config\analogStickSmashThreshold : EndMacro
Macro triggerTreshold : defaultControler\config\analogTriggerThreshold : EndMacro

Structure Bind
  ID.b
  input.b
EndStructure

Structure InputBinding
  controlStickID.b
  List buttons.Bind()
  List axises.Bind()
  List triggers.Bind()
EndStructure

Structure knownController
  config.InputConfig
  defaultBinding.InputBinding
EndStructure  
 
Structure inputData
  element.b ; the element (stick/button) responsible for this input
  stick.b ;wheter the said element was a stick or not
EndStructure

Structure AxisState
  x.l
  y.l
  z.l
EndStructure

Structure ControllerState
  buttons.b[#MAX_BUTTON_NB]
  axis.AxisState[#MAX_AXIS_NB]
EndStructure

Structure Port
  joyID.l
  *joyType.knownController
  previousState.ControllerState
  currentControlStickState.AxisState
  controlStickBuffer.b[4]
  active.b
  *figher.Fighter
EndStructure
Dim ports.Port(4)

XIncludeFile "inputlibData.pbi"

NewList inputQ.l()

Procedure setPort(port, joyID, active = 1)
  Shared ports()
  ports(port)\joyID = joyID
  ports(port)\active = active
EndProcedure

Procedure setPortFighter(port, *fighter.Fighter)
  Shared ports()
  ports(port)\figher = *fighter
  *fighter\port = port
EndProcedure

Procedure readButton(button.b, *port.Port) ;examineJoystick not included !
  Shared ports()
  ProcedureReturn JoystickButton(*port\joyID, button + 1)
EndProcedure

Procedure readAxis(*state.AxisState, axis.b, *port.Port)
  Shared ports()
  *state\x = JoystickAxisX(*port\joyID, axis, #PB_Relative)
  *state\y = JoystickAxisY(*port\joyID, axis, #PB_Relative)
  *state\z = JoystickAxisZ(*port\joyID, axis, #PB_Relative)
EndProcedure

Procedure stickDirection(*state.AxisState)
  Shared defaultControler
  x.l = *state\x
  y.l = *state\y
  If x > stickTreshold And x > Abs(y)
    ProcedureReturn #DIRECTION_RIGHT
  ElseIf y > stickTreshold And y > Abs(x)
    ProcedureReturn #DIRECTION_DOWN
  ElseIf x < -stickTreshold And -x > Abs(y)
    ProcedureReturn #DIRECTION_LEFT
  ElseIf y < -stickTreshold And -y > Abs(x)
    ProcedureReturn #DIRECTION_UP
  Else 
    ProcedureReturn #DIRECTION_NONE
  EndIf 
EndProcedure

Procedure controlStickDirection(*port.Port)
  ProcedureReturn stickDirection(*port\currentControlStickState)
EndProcedure

Procedure readTrigger(axis.b, *port.Port) ;keep in mind that a trigger is just the Z axis of an axis oh wait apparently it depends on the jystick :^)
  Shared ports()
  res.l = JoystickAxisZ(*port\joyID, axis, #PB_Relative)
  ProcedureReturn res
EndProcedure

Procedure makeInputValue(input.b, time.b, port.b, element.b, stick.b)
  ProcedureReturn input + (time << 5) + (port << 9) + (stick << 12) + (element << 13)
EndProcedure

CompilerIf #DEBUG
  Declare logInput(port.b, input.b, element.b, stick.b)
CompilerEndIf
Procedure registerInput(port, input, element = #MAX_BUTTON_NB, stick = 0)
  Shared inputQ(), defaultInputDurability()
  AddElement(inputQ()) : inputQ() = makeInputValue(input, defaultInputDurability(input), port, element, stick)
  CompilerIf #DEBUG
   logInput(port, input, element, stick)
  CompilerEndIf
EndProcedure

Procedure readInputs()
  Shared ports(), defaultControler, *port.Port
  Define state.b, id.b, axisState.AxisState

  For i = 0 To 3
    If Not ports(i)\active
      Continue  
    EndIf 
    
    *port = @ports(i)
    
    If Not ExamineJoystick(*port\joyID)
      Continue
    EndIf 
    
    ;--- Buttons
    
    ForEach defaultControler\defaultBinding\buttons()
      id = defaultControler\defaultBinding\buttons()\ID
      state = readButton(id, *port)
      
      If state And Not ports(i)\previousState\buttons[id]
        registerInput(i, defaultControler\defaultBinding\buttons()\input, id)
      EndIf
      *port\previousState\buttons[id] = state
    Next 
    
    ;--- Controlstick : smashinputs
    
    id = defaultControler\defaultBinding\controlStickID
    readAxis(*port\currentControlStickState, id, *port)
    
    If *port\currentControlStickState\x > stickSmashTreshold
      If *port\previousState\axis[id]\x < stickTreshold XOr *port\controlStickBuffer[#DIRECTION_RIGHT] = 1
        registerInput(i, #INPUT_ControlStick_SRIGHT)
        registerInput(i, #INPUT_ControlStick_RIGHT)
      EndIf 
      *port\controlStickBuffer[#DIRECTION_RIGHT] = 0
    ElseIf *port\currentControlStickState\x > stickTreshold And *port\previousState\axis[id]\x < stickTreshold And 
      *port\controlStickBuffer[#DIRECTION_LEFT] = 0
      *port\controlStickBuffer[#DIRECTION_RIGHT] = 1
    ElseIf *port\currentControlStickState\x < -stickSmashTreshold
      If *port\previousState\axis[id]\x > -stickTreshold XOr *port\controlStickBuffer[#DIRECTION_LEFT] = 1
        registerInput(i, #INPUT_ControlStick_SLEFT)
        registerInput(i, #INPUT_ControlStick_LEFT)
      EndIf 
      *port\controlStickBuffer[#DIRECTION_LEFT] = 0
    ElseIf *port\currentControlStickState\x < -stickTreshold And *port\previousState\axis[id]\x > -stickTreshold
      *port\controlStickBuffer[#DIRECTION_LEFT] = 1
      *port\controlStickBuffer[#DIRECTION_RIGHT] = 0
    Else
      *port\controlStickBuffer[#DIRECTION_LEFT] = 0
      *port\controlStickBuffer[#DIRECTION_RIGHT] = 0
    EndIf
    
    
    If *port\currentControlStickState\y > stickSmashTreshold
      If *port\previousState\axis[id]\y < stickTreshold XOr *port\controlStickBuffer[#DIRECTION_RIGHT] = 1
        registerInput(i, #INPUT_ControlStick_SDOWN)
        registerInput(i, #INPUT_ControlStick_DOWN)
      EndIf 
      *port\controlStickBuffer[#DIRECTION_DOWN] = 0
    ElseIf *port\currentControlStickState\y > stickTreshold And *port\previousState\axis[id]\y < stickTreshold
      *port\controlStickBuffer[#DIRECTION_UP] = 0
      *port\controlStickBuffer[#DIRECTION_DOWN] = 1
    ElseIf *port\currentControlStickState\y < -stickSmashTreshold
      If *port\previousState\axis[id]\y > -stickTreshold XOr *port\controlStickBuffer[#DIRECTION_LEFT] = 1
        registerInput(i, #INPUT_ControlStick_SUP)
        registerInput(i, #INPUT_ControlStick_UP)
      EndIf         
      *port\controlStickBuffer[#DIRECTION_UP] = 0
    ElseIf *port\currentControlStickState\y < -stickTreshold And *port\previousState\axis[id]\y > -stickTreshold
      *port\controlStickBuffer[#DIRECTION_UP] = 1
      *port\controlStickBuffer[#DIRECTION_DOWN] = 0
    Else
      *port\controlStickBuffer[#DIRECTION_UP] = 0
      *port\controlStickBuffer[#DIRECTION_DOWN] = 0
    EndIf 

    *port\previousState\axis[id]\x = *port\currentControlStickState\x
    *port\previousState\axis[id]\y = *port\currentControlStickState\y
    
    ;--- Sticks
    
    ForEach defaultControler\defaultBinding\axises()
      id = defaultControler\defaultBinding\axises()\ID
      readAxis(@axisState, defaultControler\defaultBinding\axises()\ID, *port)
      If axisState\x > stickTreshold And *port\previousState\axis[id]\x < stickTreshold
        registerInput(i, defaultControler\defaultBinding\axises()\input, id, 1)
        ;registerInput(i, #INPUT_ControlStick_RIGHT)
      ElseIf axisState\x < -stickTreshold And *port\previousState\axis[id]\x > -stickTreshold
        registerInput(i, defaultControler\defaultBinding\axises()\input, id, 1)
        ;registerInput(i, #INPUT_ControlStick_LEFT)
      EndIf 
        
      If axisState\y > stickTreshold And *port\previousState\axis[id]\y < stickTreshold
        registerInput(i, defaultControler\defaultBinding\axises()\input, id, 1)
        ;registerInput(i, #INPUT_ControlStick_DOWN)
      ElseIf  axisState\y < -stickTreshold And *port\previousState\axis[id]\y > -stickTreshold
        registerInput(i, defaultControler\defaultBinding\axises()\input, id, 1)
        ;registerInput(i, #INPUT_ControlStick_UP)
      EndIf 
      *port\previousState\axis[id]\x = axisState\x
      *port\previousState\axis[id]\y = axisState\y
    Next
    
    ;--- Triggers
    
    ForEach defaultControler\defaultBinding\triggers()
      axisState\z = readTrigger(defaultControler\defaultBinding\triggers()\ID, *port)
      If axisState\z > triggerTreshold And *port\previousState\axis[defaultControler\defaultBinding\triggers()\ID]\z < triggerTreshold
        registerInput(i, defaultControler\defaultBinding\triggers()\input)
      EndIf 
      *port\previousState\axis[defaultControler\defaultBinding\triggers()\ID]\z = axisState\z
    Next 
  
  Next 
  
EndProcedure

;- STATE ACTIONS
Procedure state_can_jump(state.b)
  If state = #STATE_LANDING Or state = #STATE_LANDING_LAG
    ProcedureReturn 0
  EndIf 
  ProcedureReturn 1
EndProcedure 

;- INPUT MANAGERS

Procedure inputManager_Attack(*port.Port, *info.inputData)
  Define direction.b
  Shared inputQ()
  ;todo : return 0 si le fighter est incapacitate
  If *info\stick
    state.AxisState
    readAxis(@state, *info\element, *port)
    direction = stickDirection(@state)
  Else 
    direction = controlStickDirection(*port)
  EndIf 
  If isFighterGrounded(*port\figher)
    ForEach inputQ()
      input.b = inputQ() & %11111
      If input = #INPUT_ControlStick_SDOWN Or input = #INPUT_ControlStick_SUP Or input = #INPUT_ControlStick_SLEFT Or input = #INPUT_ControlStick_SRIGHT
        DeleteElement(inputQ())
        Select input
          Case #INPUT_ControlStick_SUP
            Debug "Upsmash (" + *port\figher\name + ")"
          Case #INPUT_ControlStick_SDOWN
            Debug "Dsmash (" + *port\figher\name + ")"
          Case #INPUT_ControlStick_SLEFT, #INPUT_ControlStick_SRIGHT  ;ou reverse fsmash ? à voir si je fais un truc restrictif sur les reverse à la brawl
            Debug "FSmash (" + *port\figher\name + ")"
        EndSelect
        ProcedureReturn 1
      EndIf 
    Next 
    If *port\figher\state = #STATE_DASH
      Debug "Dash Attack (" + *port\figher\name + ")"
      ProcedureReturn 1
    EndIf
    Select direction
      Case #DIRECTION_NONE
        Debug "Jab (" + *port\figher\name + ")"
        ProcedureReturn 1
      Case #DIRECTION_RIGHT, #DIRECTION_LEFT
        Debug "Ftilt (" + *port\figher\name + ")"  ;ou reverse ftilt ? à voir si je fais un truc restrictif sur les reverse à la brawl
        ProcedureReturn 1
      Case #DIRECTION_UP
        Debug "Utilt (" + *port\figher\name + ")"
        ProcedureReturn 1
      Case #DIRECTION_DOWN
        Debug "Dtilt (" + *port\figher\name + ")"
        ProcedureReturn 1
    EndSelect 
  Else
    Select direction
      Case #DIRECTION_NONE
        Debug "Nair (" + *port\figher\name + ")"
        attack(*port\figher, #COMMAND_NAir)
        ProcedureReturn 1
      Case #DIRECTION_LEFT, #DIRECTION_RIGHT
        If direction - 1 = -*port\figher\facing
          attack(*port\figher, #COMMAND_FAir)
          Debug "Fair (" + *port\figher\name + ")"
        Else 
          attack(*port\figher, #COMMAND_BAir)
          Debug "Bair (" + *port\figher\name + ")"
        EndIf 
        ProcedureReturn 1
      Case #DIRECTION_UP
        attack(*port\figher, #COMMAND_UAir)
        Debug "Uair (" + *port\figher\name + ")"
      Case #DIRECTION_DOWN
        attack(*port\figher, #COMMAND_DAir)
        Debug "Dair (" + *port\figher\name + ")"
    EndSelect
  EndIf 
EndProcedure
*inputManagers(#INPUT_Attack) = @inputManager_Attack()

Procedure inputManager_smashStickRight(*port.Port, *info.inputData)
  Define state.b
  state = *port\figher\state
  If state = #STATE_WALK Or (state = #STATE_IDLE And *port\figher\grounded) Or state = #STATE_DASH_START
    *port\figher\facing = 1
    setState(*port\figher, #STATE_DASH_START)
  EndIf 
  If (state = #STATE_DASH Or state = #STATE_DASH_STOP) And *port\figher\facing = -1
    *port\figher\facing = 1
    setState(*port\figher, #STATE_DASH_TURN)
  EndIf 
  ProcedureReturn 1
EndProcedure
*inputManagers(#INPUT_ControlStick_RIGHT) = @inputManager_smashStickRight()

Procedure inputManager_smashStickLeft(*port.Port, *info.inputData)
  Define state.b
  state = *port\figher\state
  If state = #STATE_WALK Or (state = #STATE_IDLE And *port\figher\grounded) Or state = #STATE_DASH_START
    *port\figher\facing = -1
    setState(*port\figher, #STATE_DASH_START)
  EndIf 
  If (state = #STATE_DASH Or state = #STATE_DASH_STOP) And *port\figher\facing = 1
    *port\figher\facing = -1
    setState(*port\figher, #STATE_DASH_TURN)
  EndIf 
  ProcedureReturn 1
EndProcedure
*inputManagers(#INPUT_ControlStick_LEFT) = @inputManager_smashStickLeft()

Procedure inputManager_jump(*port.Port, *info.inputData)
  Shared defaultControler
  Define jumpType.b
  If *port\figher\grounded
    If Not state_can_jump(*port\figher\state)
      ProcedureReturn 0
    EndIf 
    If *port\figher\state = #STATE_WALK Or *port\figher\state = #STATE_DASH
      jumpType = #JUMP_WALKING
    Else 
      jumpType = #JUMP_NORMAL
    EndIf 
    setState(*port\figher, #STATE_JUMPSQUAT, jumpType + (*info\element << 2))
    ProcedureReturn 1
  ElseIf *port\figher\jumps > 0
    If Abs(*port\currentControlStickState\x) > stickTreshold
      If  Sign(*port\currentControlStickState\x) = *port\figher\facing 
        jump(*port\figher, #JUMP_WALKING, #YJUMP_DOUBLE)
      Else
        jump(*port\figher, #JUMP_BACKWARDS, #YJUMP_DOUBLE)
      EndIf 
    Else
      jump(*port\figher, #JUMP_NORMAL, #YJUMP_DOUBLE)
    EndIf
    *port\figher\jumps - 1  
  EndIf 
EndProcedure
*inputManagers(#INPUT_Jump) = @inputManager_jump()

Procedure inputManager_smashStickDown(*port.Port, *info.inputData)
  If *port\figher\state = #STATE_IDLE And *port\figher\physics\v\y < 0
    Debug "fast fall"
    *port\figher\physics\v\y = -*port\figher\character\fastFallSpeed
  EndIf 
EndProcedure    
*inputManagers(#INPUT_ControlStick_DOWN) = @inputManager_smashStickDown()

Procedure inputManager_controlStickState(*port.Port) ;not a real input manager
  Shared defaultControler
  Select *port\figher\state
    Case #STATE_IDLE
      If Abs(*port\currentControlStickState\x) > stickTreshold
        If *port\figher\grounded
          *port\figher\facing = Sign(*port\currentControlStickState\x)
          setState(*port\figher, #STATE_WALK)
        Else 
          applyAirAccel(*port\figher, Sign(*port\currentControlStickState\x))
        EndIf 
      EndIf     
    Case #STATE_WALK
      If *port\currentControlStickState\x < stickTreshold And *port\currentControlStickState\x > -stickTreshold
        setState(*port\figher, #STATE_IDLE)
      EndIf
    Case #STATE_DASH
      If *port\currentControlStickState\x < stickTreshold And *port\currentControlStickState\x > -stickTreshold
        setState(*port\figher, #STATE_DASH_STOP)
      EndIf
    Case #STATE_ATTACK
      If *port\figher\grounded
      Else 
        If Abs(*port\currentControlStickState\x) > stickTreshold
          applyAirAccel(*port\figher, Sign(*port\currentControlStickState\x))
        EndIf 
      EndIf 
  EndSelect 
EndProcedure

Procedure updateInputs()
  Shared inputQ(), ports(), *inputManagers(), *port.Port
  Define input.b, durability.b, port.b, res.b, *currentElement, info.inputData
  readInputs()
  ForEach inputQ()
    input = inputQ() & %11111
    durability = (inputQ() & %111100000) >> 5
    port = (inputQ() & %111000000000) >> 9
    info\stick = (inputQ() & %1000000000000) >> 12
    info\element = (inputQ() & %11111 << 13) >> 13
    
    If *inputManagers(input)
      *currentElement = @inputQ()
      inputManager.inputManager = *inputManagers(input)
      res = inputManager(@ports(port), @info)
      ChangeCurrentElement(inputQ(), *currentElement)
      If res
        DeleteElement(inputQ())
        Continue 
      EndIf 
    EndIf 

    durability - 1
    If durability < 1
      DeleteElement(inputQ())
    Else
      inputQ() = makeInputValue(input, durability, port, element, stick)
    EndIf 
  Next 
  For i = 0 To 3
    If Not ports(i)\active
      Continue  
    EndIf 
    *port = ports(i)
    inputManager_controlStickState(*port)
  Next
EndProcedure
  
availableJosticks.b = InitJoystick()


; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 419
; FirstLine = 382
; Folding = ----
; EnableXP