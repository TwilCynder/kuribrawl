#GRAVITY = 2.0

Procedure groundCollision(x.l, y.l) ;ajouter le fighter au cas où les ECB pourraient être différentes selon le fighter
  If y < 0
    ProcedureReturn 1  ;TODO: return un pointeur vers l'élément de décor incriminé
  EndIf 
  ProcedureReturn 0
EndProcedure

Procedure applyPhysics(*game.Game)
  Define nx.l, ny.l
  ForEach *game\fighters()
    *game\fighters()\physics\v\y - #GRAVITY
    nx = *game\fighters()\x + *game\fighters()\physics\v\x
    ny = *game\fighters()\y + *game\fighters()\physics\v\y
    If groundCollision(nx, ny)
      ny = 0
    EndIf 
    *game\fighters()\x = nx
    *game\fighters()\y = ny
  Next 
EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 19
; Folding = -
; EnableXP