  Procedure stateCallback_Jumpsquat(*fighter.Fighter, stateinfo.l)
  Shared defaultControler
  Define jumpTypeX.b, jumpTypeY.b, element.b, elementType.b, axis.AxisState
  Define *port.Port = *fighter\port
  
  jumpTypeX = stateinfo & %1
  jumpTypeY = (stateInfo & %10) >> 1
  elementType = (stateinfo & %1100) >> 2
  element = (stateinfo & %111110000) >> 4
  
  If Abs(*port\currentControlStickState\x) > *port\controllerInfo\config\analogStickThreshold And Not jumpTypeX = #JUMP_NEUTRAL
    If Sign(*port\currentControlStickState\x) <> *fighter\facing 
      jumpTypeX = #JUMP_BACKWARDS  
    Else
      jumpTypeX = #JUMP_WALKING
    EndIf 
  EndIf 
  
  If Not isElementPressed(element, elementType, *port)
    jumpTypeY = 1
  EndIf
  jump(*fighter, jumpTypeX,jumpTypeY)
EndProcedure

Procedure stateCallback_DashTurn(*fighter.Fighter, stateinfo.l)
  Define *port.Port = *fighter\port
  If Sign(*port\currentControlStickState\x) <> *fighter\facing
    setState(*fighter, #STATE_IDLE)
    *fighter\facing = -*fighter\facing
  Else
    setState(*fighter, #STATE_DASH)
  EndIf 
EndProcedure

Macro stateEnd(l)
  ((l >= 0) And *fighter\stateTimer >= l) Or (l < 0 And *fighter\currentAnimation\finished)
EndMacro


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
          If stateEnd(*fighter\character\jumpsquatDuration) 
            stateCallback_Jumpsquat(*fighter, *fighter\stateInfo)
          EndIf 
        Case #STATE_DASH_STOP
          If stateEnd(*fighter\character\dashStopDuration)  
            setState(*fighter, #STATE_IDLE)
          EndIf  
        Case #STATE_DASH_START
          If stateEnd(*fighter\character\dashStartDuration) 
            setState(*fighter, #STATE_DASH)
          EndIf 
        Case #STATE_DASH_TURN
          If stateEnd(*fighter\character\dashTurnDuration) 
            stateCallback_DashTurn(*fighter, 0)
          EndIf 
        Case #STATE_LANDING
          If stateEnd(*fighter\character\landingDuration) 
            setState(*fighter, #STATE_IDLE)
          EndIf 
        Case #STATE_LANDING_LAG
          If stateEnd(*fighter\stateInfo) 
            setState(*fighter, #STATE_IDLE)
          EndIf 
        Case #STATE_HITSTUN
          max = *fighter\stateInfo >> 1
          If *fighter\stateTimer >= max
            If *fighter\stateInfo & 1 And Not *fighter\grounded
              setState(*fighter, #STATE_TUMBLE, 0, 0)
            Else
              setState(*fighter, #STATE_IDLE, 0, *fighter\grounded)
            EndIf 
          EndIf
        Case #STATE_GUARDSTUN
          max = *fighter\stateInfo & %11111111
          If *fighter\stateTimer >= max
              setState(*fighter, #STATE_GUARD)
          EndIf
        Case #STATE_CROUCH_START
          If *fighter\currentAnimation\finished
            setState(*fighter, #STATE_CROUCH)
          EndIf 
        Case #STATE_CROUCH_STOP
          If *fighter\currentAnimation\finished
            setState(*fighter, #STATE_IDLE)
          EndIf 
        Case #STATE_GUARD_START
          If stateEnd(*fighter\character\shieldStartup)
            setState(*fighter, #STATE_GUARD)
          EndIf 
        Case #STATE_GUARD_STOP
          If stateEnd(*fighter\character\shieldEndlag) 
            setState(*fighter, #STATE_IDLE)
          EndIf
      EndSelect
      If *fighter\state = #STATE_GUARD
        *fighter\shieldSize - kuribrawl\ingameData\shieldDecay
      ElseIf Not *fighter\state = #STATE_GUARDSTUN
        *fighter\shieldSize + kuribrawl\ingameData\shieldRegen
        If *fighter\shieldSize > 1
          *fighter\shieldSize = 1.0
        EndIf 
      EndIf 
    EndIf  
  Next 
EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 107
; FirstLine = 63
; Folding = -
; EnableXP