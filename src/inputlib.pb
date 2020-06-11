XIncludeFile "inputlibConstants.pbi"

Structure InputConfig
  analogStickThreshold.l
  analogStickSmashThreshold.l
  analogTriggerThreshold.l
EndStructure
inputConfig.InputConfig
inputConfig\analogStickThreshold = 300
inputConfig\analogStickSmashThreshold = 750
inputConfig\analogTriggerThreshold = -500

Macro stickTreshold : inputConfig\analogStickThreshold : EndMacro
Macro stickSmashTreshold : inputConfig\analogStickSmashThreshold : EndMacro
Macro triggerTreshold : inputConfig\analogTriggerThreshold : EndMacro

Structure AxisState
  x.l
  y.l
  z.l
EndStructure

Structure ControllerState
  buttons.b[#MAX_BUTTON_NB]
  axis.AxisState[#MAX_AXIS_NB]
EndStructure

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

Structure inputData
  directionAxis.b
EndStructure

Structure Port
  joyID.l
  previousState.ControllerState
  currentControlStickState.AxisState
  controlStickBuffer.b[4]
  active.b
  *figher.Fighter
EndStructure

XIncludeFile "inputlibData.pbi"

NewList inputQ.l()

Dim ports.Port(4)

Procedure setPort(port, joyID, active = 1)
  Shared ports()
  ports(port)\joyID = joyID
  ports(port)\active = active
EndProcedure

Procedure setPortFighter(port, *fighter.Fighter)
  Shared ports()
  ports(port)\figher = *fighter
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
  Shared inputConfig
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

Procedure readTrigger(axis.b, *port.Port) ;keep in mind that a trigger is just the Z axis of an axis
  Shared ports()
  res.l = JoystickAxisZ(*port\joyID, axis, #PB_Relative)
  ProcedureReturn res
EndProcedure

Procedure makeInputValue(input.b, time.b, port.b, directionAxis.b)
  ProcedureReturn input + (time << 5) + (port << 9) + (directionAxis << 12)
EndProcedure

Procedure registerInput(port, input, directionAxis.b = #MAX_AXIS_NB)
  Shared InputNames(), inputQ(), defaultInputDurability()
  Debug "Port " + port + " : " + InputNames(input)
  AddElement(inputQ()) : inputQ() = makeInputValue(input, defaultInputDurability(input), port, directionAxis)
EndProcedure

Procedure readInputs()
  Shared ports(), inputConfig, defaultBind, *port.Port
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
    
    ForEach defaultBind\buttons()
      id = defaultBind\buttons()\ID
      state = readButton(id, *port)
      
      If state And Not ports(i)\previousState\buttons[id]
        registerInput(i, defaultBind\buttons()\input)
      EndIf
      *port\previousState\buttons[id] = state
    Next 
    
    ;--- Controlstick : smashinputs
    
    id = defaultBind\controlStickID
    readAxis(*port\currentControlStickState, id, *port)
    
    If *port\currentControlStickState\x > stickSmashTreshold
      If *port\previousState\axis[id]\x < stickTreshold XOr *port\controlStickBuffer[#DIRECTION_RIGHT] = 1
        registerInput(i, #INPUT_ControlStick_SRIGHT)
      EndIf 
      *port\controlStickBuffer[#DIRECTION_RIGHT] = 0
    ElseIf *port\currentControlStickState\x > stickTreshold And *port\previousState\axis[id]\x < stickTreshold And 
      *port\controlStickBuffer[#DIRECTION_LEFT] = 0
      *port\controlStickBuffer[#DIRECTION_RIGHT] = 1
    ElseIf *port\currentControlStickState\x < -stickSmashTreshold
      If *port\previousState\axis[id]\x > -stickTreshold XOr *port\controlStickBuffer[#DIRECTION_LEFT] = 1
        registerInput(i, #INPUT_ControlStick_SLEFT)
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
      EndIf 
      *port\controlStickBuffer[#DIRECTION_DOWN] = 0
    ElseIf *port\currentControlStickState\y > stickTreshold And *port\previousState\axis[id]\y < stickTreshold
      *port\controlStickBuffer[#DIRECTION_UP] = 0
      *port\controlStickBuffer[#DIRECTION_DOWN] = 1
    ElseIf *port\currentControlStickState\y < -stickSmashTreshold
      If *port\previousState\axis[id]\y > -stickTreshold XOr *port\controlStickBuffer[#DIRECTION_LEFT] = 1
        registerInput(i, #INPUT_ControlStick_SUP)
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
    
    ForEach defaultBind\axises()
      id = defaultBind\axises()\ID
      readAxis(@axisState, defaultBind\axises()\ID, *port)
      If axisState\x > stickTreshold And *port\previousState\axis[id]\x < stickTreshold
        registerInput(i, defaultBind\axises()\input, id)
        ;registerInput(i, #INPUT_ControlStick_RIGHT)
      ElseIf axisState\x < -stickTreshold And *port\previousState\axis[id]\x > -stickTreshold
        registerInput(i, defaultBind\axises()\input, id)
        ;registerInput(i, #INPUT_ControlStick_LEFT)
      EndIf 
        
      If axisState\y > stickTreshold And *port\previousState\axis[id]\y < stickTreshold
        registerInput(i, defaultBind\axises()\input, id)
        ;registerInput(i, #INPUT_ControlStick_DOWN)
      ElseIf  axisState\y < -stickTreshold And *port\previousState\axis[id]\y > -stickTreshold
        registerInput(i, defaultBind\axises()\input, id)
        ;registerInput(i, #INPUT_ControlStick_UP)
      EndIf 
      *port\previousState\axis[id]\x = axisState\x
      *port\previousState\axis[id]\y = axisState\y
    Next
    
    ;--- Triggers
    
    ForEach defaultBind\triggers()
      axisState\z = readTrigger(defaultBind\triggers()\ID, *port)
      If axisState\z > triggerTreshold And *port\previousState\axis[defaultBind\triggers()\ID]\z < triggerTreshold
        registerInput(i, defaultBind\triggers()\input)
      EndIf 
      *port\previousState\axis[defaultBind\triggers()\ID]\z = axisState\z
    Next 
  
  Next 
  
EndProcedure

Procedure inputManager_Attack(*port.Port, *info.inputData)
  Define direction.b
  Shared inputQ()
  ;todo : return 0 si le fighter est incapacitate
  If isFighterGrounded(*port\figher)
    If *port\figher\currentAnimationName = "dash"
      Debug "Dash Attack (" + *port\figher\name + ")"
      ProcedureReturn 1
    Else 
      If *info\directionAxis < #MAX_AXIS_NB
        state.AxisState
        readAxis(@state, *info\directionAxis, *port)
        direction = stickDirection(@state)
      Else 
        direction = controlStickDirection(*port)
      EndIf 

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
    EndIf 
  Else
    Select direction
      Case #DIRECTION_NONE
        Debug "Nair (" + *port\figher\name + ")"
        ProcedureReturn 1
      Case #DIRECTION_LEFT, #DIRECTION_RIGHT
        If direction = figherDirection(*port\figher) * 2
          Debug "Fair (" + *port\figher\name + ")"
        Else 
          Debug "Bair (" + *port\figher\name + ")"
        EndIf 
        ProcedureReturn 1
      Case #DIRECTION_UP
        Debug "Uair (" + *port\figher\name + ")"
      Case #DIRECTION_DOWN
        Debug "Dair (" + *port\figher\name + ")"
    EndSelect
  EndIf 
EndProcedure
*inputManagers(#INPUT_Attack) = @inputManager_Attack()

Procedure updateInputs()
  Shared inputQ(), frame, ports(), *inputManagers()
  Define input.b, durability.b, port.b, res.b, *currentElement, info.inputData
  readInputs()
  ForEach inputQ()
    input = inputQ() & %11111
    durability = (inputQ() & %111100000) >> 5
    port = (inputQ() & %111000000000) >> 9
    info\directionAxis = (inputQ() & %111000000000000) >> 12 
    ;Debug Str(input) + " "  + Str(durability) + " "+ Str(port) + " (frame : " + Str(frame) + ")"
    
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
      inputQ() = makeInputValue(input, durability, port, directionAxis)
    EndIf   
  Next 
  
EndProcedure
  
availableJosticks.b = InitJoystick()


; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 296
; FirstLine = 265
; Folding = ---
; EnableXP