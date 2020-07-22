Procedure stateCallback_Jumpsquat(*fighter.Fighter, stateinfo.l)
  Shared ports(), defaultControler
  Define jumpTypeX.b, jumpTypeY.b, element.b, elementType.b, axis.AxisState
  
  jumpTypeX = stateinfo & %1
  jumpTypeY = (stateInfo & %10) >> 1
  elementType = (stateinfo & %1100) >> 2
  element = (stateinfo & %111110000) >> 4
  
  If Abs(ports(*fighter\port)\currentControlStickState\x) > stickTreshold And Sign(ports(*fighter\port)\currentControlStickState\x) <> *fighter\facing 
    jumpTypeX = #JUMP_BACKWARDS
  EndIf 
  
  If Not isElementPressed(element, elementType, ports(*fighter\port))
    jumpTypeY = 1
  EndIf
  jump(*fighter, jumpTypeX,jumpTypeY)
EndProcedure

Procedure getStateMaxFrames(*fighter.Fighter, characterProperty.b)
  If characterProperty < 1
    ProcedureReturn animLength(*fighter\currentAnimation)
  EndIf 
  ProcedureReturn characterProperty
EndProcedure  
  
Procedure manageStates(*game.Game)
  Define *fighter.Fighter, max.b
  ForEach *game\fighters()
    *fighter = @*game\fighters()
    Select *fighter\state
      Case #STATE_JUMPSQUAT
        max = getStateMaxFrames(*fighter, *fighter\character\jumpsquatDuration)
        If *fighter\stateTimer >= max
          stateCallback_Jumpsquat(*fighter, *fighter\stateInfo)
        EndIf
      Case #STATE_DASH_STOP
        max = getStateMaxFrames(*fighter, *fighter\character\dashStopDuration)
        If *fighter\stateTimer >= max
          setState(*fighter, #STATE_IDLE)
        EndIf  
      Case #STATE_DASH_START
        max = getStateMaxFrames(*fighter, *fighter\character\dashStartDuration)
        If *fighter\stateTimer >= max
          setState(*fighter, #STATE_DASH)
        EndIf 
      Case #STATE_DASH_TURN
        max = getStateMaxFrames(*fighter, *fighter\character\dashTurnDuration)
        If *fighter\stateTimer >= max
          setState(*fighter, #STATE_DASH)
        EndIf 
      Case #STATE_LANDING
        max = getStateMaxFrames(*fighter, *fighter\character\landingDuration)

        If *fighter\stateTimer >= max
          setState(*fighter, #STATE_IDLE)
        EndIf 
      Case #STATE_LANDING_LAG
        max = getStateMaxFrames(*fighter, *fighter\character\landingDuration)

        If *fighter\stateTimer >= max
          setState(*fighter, #STATE_IDLE)
        EndIf 
      Case #STATE_LANDING
        max = getStateMaxFrames(*fighter, *fighter\stateInfo)
        If *fighter\stateTimer >= max
          setState(*fighter, #STATE_IDLE)
        EndIf 
    EndSelect
    *fighter\stateTimer + 1
  Next 
EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 5
; Folding = -
; EnableXP