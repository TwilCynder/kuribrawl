Procedure stateCallback_Jumpsquat(*fighter.Fighter, stateinfo.l)
  Shared ports(), defaultControler
  Define jumpType.b, button.b, axis.AxisState
  
  jumpType = stateinfo & %11
  button = (stateinfo & %1111100) >> 2
  
  If Abs(ports(*fighter\port)\currentControlStickState\x) > stickTreshold And Sign(ports(*fighter\port)\currentControlStickState\x) <> *fighter\facing 
    jumpType = #JUMP_BACKWARDS
  EndIf 
  
  jump(*fighter, jumpType, 1 - readButton(button, ports(*fighter\port)))
EndProcedure

Procedure getStateMaxFrames(*fighter.Fighter, characterProperty.b)
  If characterProperty < 1
    ProcedureReturn *fighter\currentAnimation\frameCount
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
    EndSelect
    *fighter\stateTimer + 1
  Next 
EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 47
; FirstLine = 9
; Folding = -
; EnableXP