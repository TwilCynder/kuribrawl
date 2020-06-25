#GRAVITY = 0.6

Procedure groundCollision(x.l, y.l) ;ajouter le fighter au cas où les ECB pourraient être différentes selon le fighter
  If y < 0
    ProcedureReturn 1  ;TODO: return un pointeur vers l'élément de décor incriminé
  EndIf 
  ProcedureReturn 0
EndProcedure

Procedure substractValue(*v1.Double, v2.d)
  Define sign.b
  sign = Sign(*v1\d)
  *v1\d = Abs(*v1\d)
  *v1\d - v2
  If *v1\d < 0.0
    *v1\d = 0.0
  Else 
    *v1\d * sign
  EndIf 
  ProcedureReturn v1
EndProcedure

Procedure applyPhysics(*game.Game)
  Shared ports()
  Define nx.l, ny.l, *fighter.Fighter

  ForEach *game\fighters()
    *fighter = @*game\fighters()
    
    If *fighter\facing = 0
      *fighter\facing = 1
    EndIf 
    
    ;--- Aplication de la traction/friction
    
    If isFighterGrounded(*fighter)
      substractValue(@*fighter\physics\v\x, *fighter\character\traction)
    Else
      substractValue(@*fighter\physics\v\x, *fighter\character\airFriction)
    EndIf 
    
    ;--- Application des vitesses dues aux états (walk/dashà
    
    Select *fighter\state
      Case #STATE_WALK  
        *fighter\physics\v\x = *fighter\character\walkSpeed * *fighter\facing
      Case #STATE_DASH
        *fighter\physics\v\x = *fighter\character\dashSpeed * *fighter\facing
      Case #STATE_DASH_START
        *fighter\physics\v\x = *fighter\character\initialDashSpeed * *fighter\facing
      Case #STATE_DASH_TURN
        *fighter\physics\v\x + *fighter\character\dashTurnAccel * *fighter\facing
    EndSelect
    
    ;--- Gravité (fastfall, etc)
    
    If *fighter\physics\v\y > -*fighter\character\maxFallSpeed
      *fighter\physics\v\y - #GRAVITY
      If *fighter\physics\v\y < -*fighter\character\maxFallSpeed
        *fighter\physics\v\y = -*fighter\character\maxFallSpeed
      EndIf 
    EndIf 
    
    ;--- Pré-application des vitesses
    
    nx = *fighter\x + *fighter\physics\v\x
    ny = *fighter\y + *fighter\physics\v\y
    
    ;--- Calcul des collisions avec le terrain
    If groundCollision(nx, ny)
      If *fighter\grounded = 0
        setState(*fighter, #STATE_LANDING)
      EndIf 
      *fighter\jumps = 1
      *fighter\grounded = 1
      *fighter\physics\v\y = 0
      ny = 0
    Else 
      *fighter\grounded = 0
    EndIf 
    
    ;--- Application des vitesses
    
    *fighter\x = nx
    *fighter\y = ny
  Next 
EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 71
; FirstLine = 33
; Folding = -
; EnableXP