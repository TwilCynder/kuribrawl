Procedure freeChampion(*champ.Champion)
  FreeMap(*champ\animations()) ;cleanse les éléments procéduralement
  FreeArray(*champ\moves())    ;clease les éléments procéduralement
  ;FreeStructure(*champ) ;à voir sir freeMap free tous les éléments (auquel cas pas besoin de ça ? à voir si on utilise freemap tout court)

EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 4
; Folding = -
; EnableXP