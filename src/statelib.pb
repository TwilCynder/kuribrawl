Procedure getStateMaxFrames(*fighter.Fighter, characterProperty.b)
  If characterProperty < 1
    ProcedureReturn animLength(*fighter\currentAnimation)
  EndIf 
  ProcedureReturn characterProperty
EndProcedure  

Procedure stateCallback_Jumpsquat(*fighter.Fighter, stateinfo.l)
  Shared ports(), defaultControler
  Define jumpTypeX.b, jumpTypeY.b, element.b, elementType.b, axis.AxisState
  
  jumpTypeX = stateinfo & %1
  jumpTypeY = (stateInfo & %10) >> 1
  elementType = (stateinfo & %1100) >> 2
  element = (stateinfo & %111110000) >> 4
  
  If Abs(ports(*fighter\port)\currentControlStickState\x) > stickTreshold And Not jumpTypeX = #JUMP_NEUTRAL
    If Sign(ports(*fighter\port)\currentControlStickState\x) <> *fighter\facing 
      jumpTypeX = #JUMP_BACKWARDS  
    Else
      jumpTypeX = #JUMP_WALKING
    EndIf 
  EndIf 
  
  If Not isElementPressed(element, elementType, ports(*fighter\port))
    jumpTypeY = 1
  EndIf
  jump(*fighter, jumpTypeX,jumpTypeY)
EndProcedure
  
Procedure manageStates(*game.Game)
  Define *fighter.Fighter, max.l
  ForEach *game\fighters()
    *fighter = @*game\fighters()
    
    If *fighter\paused > 0
      *fighter\paused - 1
    Else  
      *fighter\stateTimer + 1
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
        Case #STATE_HITSTUN
          max = *fighter\stateInfo >> 1
          If *fighter\stateTimer >= max
            If *fighter\stateInfo & 1 And Not *fighter\grounded
              setState(*fighter, #STATE_TUMBLE, 0)
            Else
              setState(*fighter, #STATE_IDLE, 0, *fighter\grounded)
            EndIf 
          EndIf
        Case #STATE_CROUCH_START
          max = animLength(*fighter\currentAnimation)
          If *fighter\stateTimer >= max
            setState(*fighter, #STATE_CROUCH)
          EndIf 
        Case #STATE_CROUCH_STOP
          max = animLength(*fighter\currentAnimation)
          If *fighter\stateTimer >= max
            setState(*fighter, #STATE_IDLE)
          EndIf 
        Case #STATE_GUARD_START
          max = getStateMaxFrames(*fighter, *fighter\stateInfo & ~%1111111)
          If *fighter\stateTimer >= max
            setState(*fighter, #STATE_GUARD, *fighter\stateInfo)
          EndIf 
        Case #STATE_GUARD_STOP
          max = getStateMaxFrames(*fighter, *fighter\character\shieldEndlag)
          If Not max 
            max = kuribrawl\variables\shieldEndlag
          EndIf
          If *fighter\stateTimer >= max
            setState(*fighter, #STATE_IDLE)
          EndIf
      EndSelect
      If *fighter\state = #STATE_GUARD
        *fighter\shieldSize - kuribrawl\variables\shieldDecay
      Else
        *fighter\shieldSize + kuribrawl\variables\shieldRegen
        If *fighter\shieldSize > 1
          *fighter\shieldSize = 1.0
        EndIf 
      EndIf 
    EndIf 
   
  Next 
EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 114
; FirstLine = 67
; Folding = -
; EnableXP