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
  elementType.b ;wheter the said element was a button, stick or trigger
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
  id.b
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
  ports(port)\id = port
EndProcedure

Procedure setPortFighter(port, *fighter.Fighter)
  Shared ports()
  ports(port)\figher = *fighter
  *fighter\port = port
EndProcedure

Procedure readButton(button.b, *port.Port) ;examineJoystick not included !
  ProcedureReturn JoystickButton(*port\joyID, button + 1)
EndProcedure

Procedure readAxis(*state.AxisState, axis.b, *port.Port)
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
  res.l = JoystickAxisZ(*port\joyID, axis, #PB_Relative)
  ProcedureReturn res
EndProcedure  

Procedure triggerPressed(axis.b, *port.Port)
  Shared defaultControler
  ProcedureReturn Bool(readTrigger(axis, *port) > triggerTreshold)
EndProcedure

Procedure makeInputValue(input.b, time.b, port.b, element.b, elementType.b)
  ProcedureReturn input + (time << 5) + (port << 9) + (elementType << 12) + (element << 14)
EndProcedure

CompilerIf #DEBUG
  Declare logInput(port.b, input.b, element.b, elementType.b, frame.l = -1)
CompilerEndIf
Procedure registerInput(port, input, element = #MAX_BUTTON_NB, elementType = #ELEMENTTYPE_BUTTON)
  Shared inputQ(), defaultInputDurability(), frame
  AddElement(inputQ()) : inputQ() = makeInputValue(input, defaultInputDurability(input), port, element, elementType)
  CompilerIf #DEBUG
   logInput(port, input, element, elementType, frame)
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
        registerInput(i, defaultControler\defaultBinding\axises()\input, id, #ELEMENTTYPE_STICK)
        ;registerInput(i, #INPUT_ControlStick_RIGHT)
      ElseIf axisState\x < -stickTreshold And *port\previousState\axis[id]\x > -stickTreshold
        registerInput(i, defaultControler\defaultBinding\axises()\input, id, #ELEMENTTYPE_STICK)
        ;registerInput(i, #INPUT_ControlStick_LEFT)
      EndIf 
        
      If axisState\y > stickTreshold And *port\previousState\axis[id]\y < stickTreshold
        registerInput(i, defaultControler\defaultBinding\axises()\input, id, #ELEMENTTYPE_STICK)
        ;registerInput(i, #INPUT_ControlStick_DOWN)
      ElseIf  axisState\y < -stickTreshold And *port\previousState\axis[id]\y > -stickTreshold
        registerInput(i, defaultControler\defaultBinding\axises()\input, id, #ELEMENTTYPE_STICK)
        ;registerInput(i, #INPUT_ControlStick_UP)
      EndIf 
      *port\previousState\axis[id]\x = axisState\x
      *port\previousState\axis[id]\y = axisState\y
    Next
    
    ;--- Triggers
    
    ForEach defaultControler\defaultBinding\triggers()
      id = defaultControler\defaultBinding\triggers()\ID
      axisState\z = readTrigger(id, *port)
      If axisState\z > triggerTreshold And *port\previousState\axis[defaultControler\defaultBinding\triggers()\ID]\z < triggerTreshold
        registerInput(i, defaultControler\defaultBinding\triggers()\input, id, #ELEMENTTYPE_TRIGGER)
      EndIf 
      *port\previousState\axis[id]\z = axisState\z
    Next 
  
  Next 
  
EndProcedure

Procedure isElementPressed(element.b, elementType.b, *port.Port) ;button pressed, stick tilted or trigger pressed
  Select elementType
    Case #ELEMENTTYPE_BUTTON
      ProcedureReturn readButton(element, *port)
    Case #ELEMENTTYPE_TRIGGER
      ProcedureReturn triggerPressed(element, *port)
  EndSelect
EndProcedure

;- STATE ACTIONS
Procedure state_can_jump(state.b)
  If state = #STATE_LANDING Or state = #STATE_LANDING_LAG Or state = #STATE_HITSTUN Or state = #STATE_TUMBLE
    ProcedureReturn 0
  EndIf 
  ProcedureReturn 1
EndProcedure 

Procedure startAttack(*fighter.Fighter, attack.b, *info.inputData = 0)
  attack(*fighter.Fighter, attack)
  If *info
    *fighter\stateInfo = *fighter\stateInfo + (*info\elementType << 8) + (*info\element << 10)
  EndIf 
EndProcedure

Macro Atk(attack)
  startAttack(*port\figher, attack, *info)
EndMacro

Procedure findDirectionalInput()
  Shared InputQ()
  ForEach inputQ()
    input.b = inputQ() & %11111
    If input = #INPUT_ControlStick_SDOWN Or input = #INPUT_ControlStick_SUP Or input = #INPUT_ControlStick_SLEFT Or input = #INPUT_ControlStick_SRIGHT
      DeleteElement(inputQ())
      ProcedureReturn input
    EndIf 
  Next 
EndProcedure

;- INPUT MANAGERS

Procedure inputManager_Attack(*port.Port, *info.inputData)
  If *port\figher\paused
    ProcedureReturn 2
  EndIf 
  Select *port\figher\state
    Case #STATE_ATTACK 
      If  *port\figher\currentMove\multiMove
        part.b = (*port\figher\stateInfo % %11100000) >> 5
        ;Debug part
        ;Debug *port\figher\currentAnimation\frame
        If (part = 0) Or (part <= ArraySize(*port\figher\currentMove\multiMove\partStartFrames())) And *port\figher\currentAnimation\frame >= *port\figher\currentMove\multiMove\partStartFrames(part - 1)
          part + 1
        Else
          ProcedureReturn 0
        EndIf 
        *port\figher\stateInfo = (*port\figher\stateInfo & (~%11100000)) + (part << 5)
        ProcedureReturn 1
      Else
        ProcedureReturn 0
      EndIf 
    Case  #STATE_HITSTUN, #STATE_LANDING_LAG
      ProcedureReturn 0
    Case #STATE_JUMPSQUAT
      ProcedureReturn 2
  EndSelect
  
  Define direction.b
  ;todo : return 0 si le fighter est incapacitate
  If *info\elementType = #ELEMENTTYPE_STICK
    state.AxisState
    readAxis(@state, *info\element, *port)
    direction = stickDirection(@state)
  Else 
    direction = controlStickDirection(*port)
  EndIf 
  If isFighterGrounded(*port\figher)
    input.b = findDirectionalInput()
    Select input
      Case #INPUT_ControlStick_SUP
        Debug "Upsmash (" + *port\figher\name + ")"
        ProcedureReturn 1
      Case #INPUT_ControlStick_SDOWN
        Debug "Dsmash (" + *port\figher\name + ")"
        ProcedureReturn 1
      Case #INPUT_ControlStick_SLEFT, #INPUT_ControlStick_SRIGHT  ;ou reverse fsmash ? à voir si je fais un truc restrictif sur les reverse à la brawl
        Debug "FSmash (" + *port\figher\name + ")"
        Atk(#COMMAND_Jab)
        ProcedureReturn 1
      Default 
    EndSelect
    If *port\figher\state = #STATE_DASH
      Debug "Dash Attack (" + *port\figher\name + ")"
      Atk(#COMMAND_Jab)
      ProcedureReturn 1
    EndIf
    Select direction
      Case #DIRECTION_NONE
        Debug "Jab (" + *port\figher\name + ")"
        Atk(#COMMAND_Jab)
        ProcedureReturn 1
      Case #DIRECTION_RIGHT, #DIRECTION_LEFT
        Debug "Ftilt (" + *port\figher\name + ")"  ;ou reverse ftilt ? à voir si je fais un truc restrictif sur les reverse à la brawl
        Atk(#COMMAND_Jab)
        ProcedureReturn 1
      Case #DIRECTION_UP
        Atk(#COMMAND_UTilt)
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
  If *port\figher\paused
    ProcedureReturn 0
  EndIf 
  Define state.b
  state = *port\figher\state
  If state = #STATE_WALK Or (state = #STATE_IDLE And *port\figher\grounded) Or state = #STATE_DASH_START
    Debug "oui"
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
  If *port\figher\paused
    ProcedureReturn 0
  EndIf 
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

Procedure jumpManager(*port.Port, *info.inputData, typeY.b)
  If *port\figher\paused
    ProcedureReturn 2
  EndIf 
  
  Shared defaultControler
  Define jumpType.b, jumpElement.b, jumpElementType.b
  If *port\figher\state = #STATE_JUMPSQUAT
    jumpElementType = (*port\figher\stateInfo & %1100) >> 2
    jumpElement = (*port\figher\stateInfo & %111110000) >> 4
    If Not (*info\element = jumpElement And *info\elementType = jumpElementType)
      *port\figher\stateInfo = *port\figher\stateInfo | %10
      ProcedureReturn 1  
    EndIf 
  EndIf 
  
  If Not state_can_jump(*port\figher\state)
    ProcedureReturn 0
  EndIf 
  
  If *port\figher\state = #STATE_ATTACK
    If *port\figher\stateTimer > 3 Or Not *port\figher\grounded
      ProcedureReturn 0
    Else
      Debug "attack cancel"
      *port\figher\physics\v\x = 0
      jumpType = #JUMP_NORMAL
      typeY = #YJUMP_SHORT
      *port\figher\facing = Sign(*port\currentControlStickState\x)
      elementType.b = (*port\figher\stateInfo & %1100000000) >> 8
      element.b = (*port\figher\stateInfo & %111110000000000) >> 10
      registerInput(*port\id, #INPUT_Attack, elementType, element)
    EndIf 
  EndIf 
  
  If *port\figher\grounded
    If (*port\figher\state = #STATE_WALK Or *port\figher\state = #STATE_DASH) And Not jumpType
      jumpType = #JUMP_WALKING
    Else 
      jumpType = #JUMP_NORMAL
    EndIf 
    setState(*port\figher, #STATE_JUMPSQUAT, jumpType + (typeY << 1) + (*info\elementType << 2) + (*info\element << 4))
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
  ProcedureReturn 1
EndProcedure  
  
Procedure inputManager_jump(*port.Port, *info.inputData)
  ProcedureReturn jumpManager(*port, *info, 0)
EndProcedure
*inputManagers(#INPUT_Jump) = @inputManager_jump()

Procedure inputManager_shorthop(*port.Port, *info.inputData)
  ProcedureReturn jumpManager(*port, *info, #YJUMP_SHORT)
EndProcedure
*inputManagers(#INPUT_Shorthop) = @inputManager_shorthop()

Procedure inputManager_smashStickDown(*port.Port, *info.inputData)
  If Not *port\figher\grounded And *port\figher\physics\v\y < 0
    Debug "fast fall"
    *port\figher\physics\v\y = -*port\figher\character\fastFallSpeed
  EndIf 
EndProcedure    
*inputManagers(#INPUT_ControlStick_DOWN) = @inputManager_smashStickDown()

Procedure inputManager_guard(*port.Port, *info.inputData)
  If Not *port\figher\grounded Or *port\figher\state = #STATE_ATTACK
    ProcedureReturn 0
  EndIf 
  Select findDirectionalInput()
    Case #INPUT_ControlStick_SDOWN
    Default 
      shieldStartup.b = *port\figher\character\shieldStartup
      If Not shieldStartup
        shieldStartup = kuribrawl\variables\shieldStartup
      EndIf 
      setState(*port\figher, #STATE_GUARD_START, *info\elementType + (*info\element << 2) + (shieldStartup << 7))
  EndSelect
  ProcedureReturn 1
EndProcedure
*inputManagers(#INPUT_Guard) = @inputManager_guard()

Procedure checkControlStickState(*port.Port) ;not a real input manager
  Shared defaultControler
  Select *port\figher\state
    Case #STATE_CROUCH_STOP
      If *port\currentControlStickState\y > stickTreshold
        setState(*port\figher, #STATE_CROUCH_START)
      EndIf 
    Case #STATE_CROUCH
      If *port\currentControlStickState\y < stickTreshold
        setState(*port\figher, #STATE_CROUCH_STOP)
      EndIf       
    Case #STATE_IDLE
      If *port\currentControlStickState\y > stickTreshold And *port\figher\grounded
        crouch(*port\figher)
      EndIf   
      If Abs(*port\currentControlStickState\x) > stickTreshold
        applyAirDrift(*port\figher, Sign(*port\currentControlStickState\x))
      EndIf 
    Case #STATE_TUMBLE
      If Abs(*port\currentControlStickState\x) > stickTreshold
        applyAirDrift(*port\figher, Sign(*port\currentControlStickState\x))
      EndIf
    Case #STATE_WALK
      If *port\currentControlStickState\x < stickTreshold And *port\currentControlStickState\x > -stickTreshold
        setState(*port\figher, #STATE_IDLE)
      EndIf
    Case #STATE_DASH
      If Abs(*port\currentControlStickState\x) < stickTreshold
        setState(*port\figher, #STATE_DASH_STOP)
      Else
        If Sign(*port\currentControlStickState\x) <> *port\figher\facing
          *port\figher\facing = Sign(*port\currentControlStickState\x)
          setstate(*port\figher, #STATE_IDLE)
        EndIf 
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

Procedure checkInputReleases(*port.Port)
  Select *port\figher\state
    Case #STATE_GUARD
      If *port\figher\stateTimer >= kuribrawl\variables\minimumShieldDuration And Not isElementPressed((*port\figher\stateInfo & %1111100) >> 2, *port\figher\stateInfo & %11, *port)
        shieldEndlag.b = *port\figher\character\shieldEndlag
        If Not shieldEndlag
          shieldEndlag = kuribrawl\variables\shieldEndlag
        EndIf 
        setState(*port\figher, #STATE_GUARD_STOP, shieldEndlag)
      EndIf 
  EndSelect 
EndProcedure

;input structure : 
; - 5 bits : input
; - 4 bits : durability
; - 3 bits : port
; - 2 bits : elementType
; - 5 bits : element

Procedure updateInputs()
  Shared inputQ(), ports(), *inputManagers(), *port.Port
  Define input.b, durability.b, port.b, res.b, *currentElement, info.inputData
  readInputs()
  
  ForEach inputQ()
    input = inputQ() & %11111
    durability = (inputQ() & %111100000) >> 5
    port = (inputQ() & %111000000000) >> 9
    info\elementType = (inputQ() & %11000000000000) >> 12
    info\element = (inputQ() & %11111 << 14) >> 14
    
    If *inputManagers(input)
      *currentElement = @inputQ()
      inputManager.inputManager = *inputManagers(input)
      res = inputManager(@ports(port), @info)
      ChangeCurrentElement(inputQ(), *currentElement)
    Else
      res = 0
    EndIf
    Select res
      Case 1
        DeleteElement(inputQ())
        Continue 
      Case 0
        durability - 1
        If durability < 1
          DeleteElement(inputQ())
        Else
          inputQ() = makeInputValue(input, durability, port, info\element, info\elementType)
        EndIf 
    EndSelect  
  Next 
    
  For i = 0 To 3
    If Not ports(i)\active
      Continue  
    EndIf 
    *port = ports(i)
    checkControlStickState(*port)
    checkInputReleases(*port)
  Next
EndProcedure
  
availableJosticks.b = InitJoystick()


; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 509
; FirstLine = 490
; Folding = ------
; EnableXP