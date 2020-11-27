#GRAVITY = 0.6

Procedure groundCollision(*game.Game, x.l, y.l, nx.l, ny.l) ;ajouter le fighter au cas où les ECB pourraient être différentes selon le fighter
  Define *plat.Platform
  ForEach *game\currentStage\platforms()
    *plat = @*game\currentStage\platforms()
    If x > *plat\model\x And x < *plat\model\x + *plat\model\w
      If y >= *plat\model\y And ny <= *plat\model\y
        ProcedureReturn *plat
      EndIf 
    EndIf 
  Next
  ProcedureReturn 0
EndProcedure

Procedure landCallback(*fighter.Fighter)
  If *fighter\state = #STATE_ATTACK
    lag = *fighter\character\moves(*fighter\stateInfo & %11111)\landLag
    If lag
      setState(*fighter, #STATE_LANDING_LAG, lag)
    EndIf 
  Else
    setState(*fighter, #STATE_LANDING)
  EndIf 
EndProcedure

Procedure applyAirAccel(*fighter.Fighter, direction.b)
  If Not (Sign(*fighter\physics\v\x) = direction And Abs(*fighter\physics\v\x) > *fighter\character\maxAirSpeed )
    *fighter\physics\v\x + *fighter\character\airAcceleration * direction
    If *fighter\physics\v\x > *fighter\character\maxAirSpeed 
      *fighter\physics\v\x = *fighter\character\maxAirSpeed 
    ElseIf *fighter\physics\v\x < -*fighter\character\maxAirSpeed 
      *fighter\physics\v\x = -*fighter\character\maxAirSpeed 
    EndIf
  EndIf 
EndProcedure

Procedure applyAirDrift(*fighter.Fighter, direction)
    If *fighter\grounded
      *fighter\facing = direction
      setState(*fighter, #STATE_WALK)
    Else 
      applyAirAccel(*fighter, direction)
    EndIf 
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

Procedure groundToAir(*fighter.Fighter)
  If Not (*fighter\state = #STATE_HITSTUN Or (*fighter\state = #STATE_IDLE And *fighter\stateInfo & 1))
    setState(*fighter, #STATE_IDLE)
  EndIf 
EndProcedure

Procedure knockbackBounce(*fighter.Fighter)
  *fighter\physics\v\y = (-*fighter\physics\v\y) * 0.7
  setAnimation(*fighter, "hurtbounce", 0, -Sign(*fighter\physics\v\x))
EndProcedure

Procedure applyPhysics(*game.Game)
  Define nx.l, ny.l, *fighter.Fighter
  Define oldSpeed.Vector

  ForEach *game\fighters()
    *fighter = @*game\fighters()
    
    If *fighter\paused > 0
      Continue
    EndIf 
    
    If *fighter\facing = 0
      *fighter\facing = 1
    EndIf 
    
    oldSpeed\x = *fighter\physics\v\x
    oldSpeed\y = *fighter\physics\v\y
    
    ;--- Aplication de la traction/friction
    
    If isFighterGrounded(*fighter)
      substractValue(@*fighter\physics\v\x, *fighter\character\traction)
    Else
      substractValue(@*fighter\physics\v\x, *fighter\character\airFriction)
    EndIf 
    
    ;--- Application des vitesses dues aux états (walk/dash)
    
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
    
    ;Détection de "fin de mouvement" (le signe de la vitesse X change)
    If (*fighter\state = #STATE_HITSTUN Or *fighter\state = #STATE_GUARDSTUN) And *fighter\grounded And Sign(oldSpeed\x) <> Sign(*fighter\physics\v\x)
      If *fighter\state = #STATE_HITSTUN
        ;TODO : si le knockback était de type tumble, placer en animation au sol
        setstate(*fighter, #STATE_IDLE)
      Else
        setstate(*fighter, #STATE_GUARD, (*fighter\stateInfo % 111111100000000) >> 8)
      EndIf 
    EndIf
    
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
    Define *plat.Platform
    *plat = groundCollision(*game, *fighter\x, *fighter\y, nx, ny)
    If *plat
      ny = *plat\model\y
      If *fighter\physics\v\y < -15 And (*fighter\state = #STATE_TUMBLE Or *fighter\state = #STATE_HITSTUN)
        knockbackBounce(*fighter)
      Else  
        If *fighter\grounded = 0
          landCallback(*fighter)
        EndIf 
        *fighter\jumps = 5
        *fighter\grounded = 1
        *fighter\physics\v\y = 0
      EndIf 
    Else 
      If *fighter\grounded = 1
        groundToAir(*fighter)
      EndIf 
      *fighter\grounded = 0
    EndIf  
    
    ;--- Application des vitesses
    
    *fighter\x = nx
    *fighter\y = ny
    
   ;--- Vérification de sortie de blastzone
      ;TODO: calculer l'angle de sortie
    If ny < 0 Or nx < 0 Or nx > *game\currentStage\model\w Or (ny > *game\currentStage\model\h And *fighter\state = #STATE_HITSTUN)
      Debug "DEATH"
      death(*fighter, *game)
    EndIf
    
  Next 
EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 26
; FirstLine = 2
; Folding = --
; EnableXP