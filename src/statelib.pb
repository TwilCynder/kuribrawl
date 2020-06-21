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

Procedure manageStates(*game.Game)
  Define *fighter.Fighter, max.b
  ForEach *game\fighters()
    *fighter = @*game\fighters()
    Select *fighter\state
      Case #STATE_JUMPSQUAT
        max = *fighter\character\jumpsquatDuration
        If max < 1
          max = *fighter\currentAnimation\frameCount
        EndIf 
        If *fighter\stateTimer >= max
          stateCallback_Jumpsquat(*fighter, *fighter\stateInfo)
        EndIf
      Case #STATE_DASH_STOP
        max = *fighter\character\dashStopDuration
        If max < 1
          max = *fighter\currentAnimation\frameCount
        EndIf 
        If *fighter\stateTimer >= max
          setState(*fighter, #STATE_IDLE)
        EndIf  
      Case #STATE_DASH_START
        max = *fighter\character\dashStartDuration
        If max < 1
          max = *fighter\currentAnimation\frameCount
        EndIf 
        If *fighter\stateTimer >= max
          setState(*fighter, #STATE_DASH)
        EndIf 
      Case #STATE_DASH_TURN
        max = *fighter\character\dashTurnDuration
        If max < 1
          max = *fighter\currentAnimation\frameCount
        EndIf 
        If *fighter\stateTimer >= max
          setState(*fighter, #STATE_DASH)
        EndIf 
    EndSelect
    *fighter\stateTimer + 1
  Next 
EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 1
; Folding = -
; EnableXP