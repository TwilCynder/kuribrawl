XIncludeFile "inputlibConstants.pbi"

Structure InputConfig
  analogStickThreshold.l
  analogStickSmashThreshold.l
  analogTriggerThreshold.l
EndStructure

Macro stickTreshold : *controller\config\analogStickThreshold : EndMacro
Macro stickSmashTreshold : *controller\config\analogStickSmashThreshold : EndMacro
Macro triggerTreshold : *controller\config\analogTriggerThreshold : EndMacro

Structure Bind
  ID.b
  input.b
EndStructure

Structure InputBinding
  controlStickID.b
  secondaryStickID.b ;utile pour le shield tilt
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
  *game.Game
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
  previousState.ControllerState
  currentControlStickState.AxisState
  controlStickBuffer.b[4]
  active.b
  *controllerInfo.knownController
  *figher.Fighter

  guardPressed.b
EndStructure
Dim ports.Port(4)

XIncludeFile "inputlibData.pbi"

Procedure setPort(port, joyID, active = 1)
  Shared ports(), defaultControler
  ports(port)\joyID = joyID
  ports(port)\active = active
  ports(port)\id = port
  ports(port)\controllerInfo = @defaultControler
EndProcedure

Procedure setPortFighter(port, *fighter.Fighter)
  Shared ports()
  ports(port)\figher = *fighter
  *fighter\port = ports(port)
EndProcedure

Procedure readButton(button.b, *port.Port) ;examineJoystick not included !
  ProcedureReturn JoystickButton(*port\joyID, button + 1)
EndProcedure

Procedure readAxis(*state.AxisState, axis.b, *port.Port)
  *state\x = JoystickAxisX(*port\joyID, axis, #PB_Relative)
  *state\y = JoystickAxisY(*port\joyID, axis, #PB_Relative)
  *state\z = JoystickAxisZ(*port\joyID, axis, #PB_Relative)
EndProcedure

Procedure stickDirection(*state.AxisState, *controllerConfig.InputConfig)
  Shared defaultControler
  x.l = *state\x
  y.l = *state\y
  If x > *controllerConfig\analogStickThreshold And x > Abs(y)
    ProcedureReturn #DIRECTION_RIGHT
  ElseIf y > *controllerConfig\analogStickThreshold And y > Abs(x)
    ProcedureReturn #DIRECTION_DOWN
  ElseIf x < -*controllerConfig\analogStickThreshold And -x > Abs(y)
    ProcedureReturn #DIRECTION_LEFT
  ElseIf y < -*controllerConfig\analogStickThreshold And -y > Abs(x)
    ProcedureReturn #DIRECTION_UP
  Else
    ProcedureReturn #DIRECTION_NONE
  EndIf
EndProcedure

Procedure controlStickDirection(*port.Port)
  ProcedureReturn stickDirection(*port\currentControlStickState, *port\controllerInfo\config)
EndProcedure

Procedure readTrigger(axis.b, *port.Port) ;keep in mind that a trigger is just the Z axis of an axis oh wait apparently it depends on the jystick :^)
  res.l = JoystickAxisZ(*port\joyID, axis, #PB_Relative)
  ProcedureReturn res
EndProcedure

Procedure triggerPressed(axis.b, *port.Port)
  ProcedureReturn Bool(readTrigger(axis, *port) > *port\controllerInfo\config\analogTriggerThreshold)
EndProcedure

Procedure makeInputValue(input.b, time.b, port.b, element.b, elementType.b)
  ProcedureReturn input + (time << 5) + (port << 9) + (elementType << 12) + (element << 14)
EndProcedure

CompilerIf #DEBUG
  Declare logInput(port.b, input.b, element.b, elementType.b, frame.l = -1)
CompilerEndIf
Procedure registerInput(*game.Game, port, input, element = #MAX_BUTTON_NB, elementType = #ELEMENTTYPE_BUTTON)
  Shared defaultInputDurability(), frame
  addElementVal(*game\inputQ(), makeInputValue(input, defaultInputDurability(input), port, element, elementType))
  CompilerIf #DEBUG
   logInput(port, input, element, elementType, frame)
  CompilerEndIf
EndProcedure

Procedure readInputs(*game.Game)
  Shared ports(), *port.Port
  Define state.b, id.b, axisState.AxisState, *controller.knownController, *binding.InputBinding

  For i = 0 To 3
    *port = @ports(i)

    If Not (*port\active And *game)
      Continue
    EndIf

    *port\guardPressed = #False

    If Not ExamineJoystick(*port\joyID)
      Continue
    EndIf

    *controller = *port\controllerInfo
    *binding = *controller\defaultBinding

    ;--- Buttons

    ForEach *binding\buttons()
      id = *binding\buttons()\ID
      state = readButton(id, *port)

      If state And *binding\buttons()\input = #INPUT_Guard
        *port\guardPressed = #True
      EndIf

      If state And Not *port\previousState\buttons[id]
        registerInput(*game, i, *binding\buttons()\input, id)
      EndIf
      *port\previousState\buttons[id] = state
    Next

    ;--- Controlstick : smashinputs

    id = *binding\controlStickID
    readAxis(*port\currentControlStickState, id, *port)

    If *port\currentControlStickState\x > stickSmashTreshold
      If *port\previousState\axis[id]\x < stickTreshold XOr *port\controlStickBuffer[#DIRECTION_RIGHT] = 1
        registerInput(*game, i, #INPUT_ControlStick_SRIGHT)
        registerInput(*game, i, #INPUT_ControlStick_RIGHT)
      EndIf
      *port\controlStickBuffer[#DIRECTION_RIGHT] = 0
    ElseIf *port\currentControlStickState\x > stickTreshold And *port\previousState\axis[id]\x < stickTreshold And
      *port\controlStickBuffer[#DIRECTION_LEFT] = 0
      *port\controlStickBuffer[#DIRECTION_RIGHT] = 1
    ElseIf *port\currentControlStickState\x < -stickSmashTreshold
      If *port\previousState\axis[id]\x > -stickTreshold XOr *port\controlStickBuffer[#DIRECTION_LEFT] = 1
        registerInput(*game, i, #INPUT_ControlStick_SLEFT)
        registerInput(*game, i, #INPUT_ControlStick_LEFT)
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
        registerInput(*game, i, #INPUT_ControlStick_SDOWN)
        registerInput(*game, i, #INPUT_ControlStick_DOWN)
      EndIf
      *port\controlStickBuffer[#DIRECTION_DOWN] = 0
    ElseIf *port\currentControlStickState\y > stickTreshold And *port\previousState\axis[id]\y < stickTreshold
      *port\controlStickBuffer[#DIRECTION_UP] = 0
      *port\controlStickBuffer[#DIRECTION_DOWN] = 1
    ElseIf *port\currentControlStickState\y < -stickSmashTreshold
      If *port\previousState\axis[id]\y > -stickTreshold XOr *port\controlStickBuffer[#DIRECTION_LEFT] = 1
        registerInput(*game, i, #INPUT_ControlStick_SUP)
        registerInput(*game, i, #INPUT_ControlStick_UP)
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

    ForEach *binding\axises()
      id = *binding\axises()\ID
      readAxis(@axisState, *binding\axises()\ID, *port)
      If axisState\x > stickTreshold And *port\previousState\axis[id]\x < stickTreshold
        registerInput(*game, i, *binding\axises()\input, id, #ELEMENTTYPE_STICK)
        ;registerInput(*game, i, #INPUT_ControlStick_RIGHT)
      ElseIf axisState\x < -stickTreshold And *port\previousState\axis[id]\x > -stickTreshold
        registerInput(*game, i, *binding\axises()\input, id, #ELEMENTTYPE_STICK)
        ;registerInput(*game, i, #INPUT_ControlStick_LEFT)
      EndIf

      If axisState\y > stickTreshold And *port\previousState\axis[id]\y < stickTreshold
        registerInput(*game, i, *binding\axises()\input, id, #ELEMENTTYPE_STICK)
        ;registerInput(*game, i, #INPUT_ControlStick_DOWN)
      ElseIf  axisState\y < -stickTreshold And *port\previousState\axis[id]\y > -stickTreshold
        registerInput(*game, i, *binding\axises()\input, id, #ELEMENTTYPE_STICK)
        ;registerInput(*game, i, #INPUT_ControlStick_UP)
      EndIf
      *port\previousState\axis[id]\x = axisState\x
      *port\previousState\axis[id]\y = axisState\y
    Next

    ;--- Triggers

    ForEach *binding\triggers()
      id = *binding\triggers()\ID
      axisState\z = readTrigger(id, *port)

      If axisState\z > triggerTreshold And *binding\triggers()\input = #INPUT_Guard
        *port\guardPressed = #True
      EndIf

      If axisState\z > triggerTreshold And *port\previousState\axis[*binding\triggers()\ID]\z < triggerTreshold
        registerInput(*game, i, *binding\triggers()\input, id, #ELEMENTTYPE_TRIGGER)
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

Procedure startGuard(*fighter.Fighter, *info.inputData = 0)
  shieldStartup.b = *fighter\character\shieldStartup
  setState(*fighter, #STATE_GUARD_START, shieldStartup)
EndProcedure

Macro Atk(attack)
  startAttack(*port\figher, attack, *info)
EndMacro

Procedure findDirectionalInput(*game.Game, port.b)
  ForEach *game\inputQ()
    inputPort.b = (*game\inputQ() & %111000000000) >> 9
    If inputPort = port
      input.b = *game\inputQ() & %11111
      If input = #INPUT_ControlStick_SDOWN Or input = #INPUT_ControlStick_SUP Or input = #INPUT_ControlStick_SLEFT Or input = #INPUT_ControlStick_SRIGHT
        DeleteElement(*game\inputQ())
        ProcedureReturn input
      EndIf
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
        If (part = 0) Or (part <= ArraySize(*port\figher\currentMove\multiMove\partStartFrames())) And *port\figher\currentAnimation\currentFrame >= *port\figher\currentMove\multiMove\partStartFrames(part - 1)
          part + 1
        Else
          ProcedureReturn 0
        EndIf
        *port\figher\stateInfo = (*port\figher\stateInfo & (~%11100000)) + (part << 5)
        ProcedureReturn 1
      Else
        ProcedureReturn 0
      EndIf
    Case  #STATE_HITSTUN, #STATE_LANDING_LAG, #STATE_GUARD, #STATE_GUARDSTUN
      ProcedureReturn 0
    Case #STATE_JUMPSQUAT
      ProcedureReturn 2
  EndSelect

  Define direction.b
  ;todo : return 0 si le fighter est incapacitate
  If *info\elementType = #ELEMENTTYPE_STICK
    state.AxisState
    readAxis(@state, *info\element, *port)
    direction = stickDirection(@state, *port\controllerInfo\config)
  Else
    direction = controlStickDirection(*port)
  EndIf
  If isFighterGrounded(*port\figher)
    input.b = findDirectionalInput(*info\game, *port\id)
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
  If state = #STATE_WALK Or (state = #STATE_IDLE And *port\figher\grounded) Or (state = #STATE_DASH_START And *port\figher\facing = -1)
    *port\figher\facing = 1
    setState(*port\figher, #STATE_DASH_START, 0, 2)
  EndIf
  If (state = #STATE_DASH Or state = #STATE_DASH_STOP) And *port\figher\facing = -1
    *port\figher\facing = 1
    setState(*port\figher, #STATE_DASH_TURN)
    Debug *port\figher\updateAnim
  EndIf
  ProcedureReturn 1
EndProcedure
*inputManagers(#INPUT_ControlStick_RIGHT) = @inputManager_smashStickRight()

Procedure inputManager_smashStickLeft(*port.Port, *info.inputData)
  Define state.b
  state = *port\figher\state
  If *port\figher\paused Or state = #STATE_DASH_TURN
    ProcedureReturn 0
  EndIf
  If state = #STATE_WALK Or (state = #STATE_IDLE And *port\figher\grounded) Or (state = #STATE_DASH_START And *port\figher\facing = 1)
    *port\figher\facing = -1
    setState(*port\figher, #STATE_DASH_START, 0, 2)
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
      registerInput(*info\game, *port\id, #INPUT_Attack, elementType, element)
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
    If Abs(*port\currentControlStickState\x) > *port\controllerInfo\config\analogStickThreshold
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
  If findDirectionalInput(*info\game, *port\id) = #INPUT_ControlStick_SDOWN Or *port\currentControlStickState\y < -*port\controllerInfo\config\analogStickThreshold

    Debug "spot dodge"
  ElseIf  Not *port\figher\state = #STATE_GUARD
    startGuard(*port\figher, *info)
    ProcedureReturn 1
  EndIf
EndProcedure
*inputManagers(#INPUT_Guard) = @inputManager_guard()

Procedure checkSticksState(*port.Port)
  Define *controller.knownController = *port\controllerInfo
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
      If *port\guardPressed And *port\figher\grounded
        startGuard(*port\figher)
      EndIf
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
      ElseIf Sign(*port\currentControlStickState\x) <> *port\figher\facing
        setState(*port\figher, #STATE_DASH_TURN)
        *port\figher\facing = -*port\figher\facing
      EndIf
    Case #STATE_ATTACK
      If *port\figher\grounded
      Else
        If Abs(*port\currentControlStickState\x) > stickTreshold
          applyAirAccel(*port\figher, Sign(*port\currentControlStickState\x))
        EndIf
      EndIf
    Case #STATE_GUARD
      x.d = JoystickAxisX(*port\joyID, *port\controllerInfo\defaultBinding\secondaryStickID, #PB_Relative)
      y.d = JoystickAxisY(*port\joyID, *port\controllerInfo\defaultBinding\secondaryStickID, #PB_Relative)
      norm.l = Sqr(x * x + y * y)
      If norm > stickTreshold
        factor.d = getShieldTiltFactor(*port\figher)
        Debug factor
        *port\figher\shieldPosition\x = (x / 1000) * factor
        *port\figher\shieldPosition\y = (y / 1000) * factor
      Else
        *port\figher\shieldPosition\x = 0
        *port\figher\shieldPosition\y = 0
      EndIf
  EndSelect
EndProcedure

Procedure checkInputReleases(*port.Port)
  Select *port\figher\state
    Case #STATE_GUARD
      If *port\figher\stateTimer >= kuribrawl\ingameData\minimumShieldDuration And Not *port\guardPressed
        shieldEndlag.b = *port\figher\character\shieldEndlag
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

Procedure updateInputs(*game.Game)
  Shared inputCode.l, ports(), *inputManagers(), *port.Port
  Define input.b, durability.b, port.b, res.b, *currentElement, info.inputData
  For i = 0 To 3
    If Not (ports(i)\active And ports(i)\figher)
      Continue
    EndIf
    *port = ports(i)
    checkInputReleases(*port)
  Next

  ForEach *game\inputQ()
    inputCode = *game\inputQ()
    input = inputCode & %11111
    durability = (inputCode & %111100000) >> 5
    port = (inputCode & %111000000000) >> 9
    info\elementType = (inputCode & %11000000000000) >> 12
    info\element = (inputCode & %11111 << 14) >> 14
    info\game = *game

    If *inputManagers(input)
      *currentElement = @*game\inputQ()
      inputManager.inputManager = *inputManagers(input)
      res = inputManager(@ports(port), @info)
      ChangeCurrentElement(*game\inputQ(), *currentElement)
    Else
      res = 0
    EndIf
    Select res
      Case 1
        DeleteElement(*game\inputQ())
        Continue
      Case 0
        durability - 1
        If durability < 1
          DeleteElement(*game\inputQ())
        Else
          *game\inputQ() = makeInputValue(input, durability, port, info\element, info\elementType)
        EndIf
    EndSelect
  Next

  For i = 0 To 3
    If Not (ports(i)\active And ports(i)\figher)
      Continue
    EndIf
    *port = ports(i)
    checkSticksState(*port)
  Next

EndProcedure

availableJosticks.b = InitJoystick()


; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 162
; FirstLine = 141
; Folding = ------
; EnableXP