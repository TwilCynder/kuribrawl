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
    If isFighterGrounded(*fighter)
      substractValue(@*fighter\physics\v\x, *fighter\character\traction)
    EndIf 
    
    If *fighter\facing = 0
      *fighter\facing = 1
    EndIf 
    
    Select *fighter\state
      Case #STATE_WALK  
        *fighter\physics\v\x = *fighter\character\walkSpeed * *fighter\facing
      Case #STATE_DASH
        *fighter\physics\v\x = *fighter\character\dashSpeed * *fighter\facing
    EndSelect
    *fighter\physics\v\y - #GRAVITY
    nx = *fighter\x + *fighter\physics\v\x
    ny = *fighter\y + *fighter\physics\v\y
    If groundCollision(nx, ny)
      *fighter\grounded = 1
      *fighter\physics\v\y = 0
      ny = 0
    Else 
      *fighter\grounded = 0
    EndIf 

    *fighter\x = nx
    *fighter\y = ny
  Next 
EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 40
; FirstLine = 6
; Folding = -
; EnableXP