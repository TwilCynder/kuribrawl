;freeFrame : no need (only pointers/numbers)

Procedure freePlatform(*platform.Platform)
EndProcedure

Procedure freeStage(*stage.Stage)
  ForEach *stage\platforms()
    freePlatform(*stage\platforms())
  Next
  FreeList(*stage\platforms())
 
  FreeStructure(*stage)
EndProcedure

Procedure freeFighter(*fighter.Fighter)
  
  FreeList(*fighter\fightersHit())
  
EndProcedure

Procedure freeGame(*game.Game)
  freeStage(*game\currentStage)
  ForEach *game\fighters()
    freeFighter(*game\fighters())
  Next
  FreeList(*game\fighters())
  FreeStructure(*game)
EndProcedure

Procedure freeChampion(*champ.Champion)
  FreeMap(*champ\animations()) ;cleanse les éléments procéduralement
  FreeArray(*champ\moves())    ;clease les éléments procéduralement
  ;FreeStructure(*champ) ;à voir sir freeMap free tous les éléments (auquel cas pas besoin de ça ? à voir si on utilise freemap tout court)

EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 10
; Folding = -
; EnableXP