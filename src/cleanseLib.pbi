;freeFrame : no need (only pointers/numbers)

Procedure freeAnimation(*animation.Animation)
  FreeList(*animation\frames())
EndProcedure

Procedure freePlatform(*platform.Platform)
  freeAnimation(*platform\animation)
EndProcedure

Procedure freeStage(*stage.Stage)
  ForEach *stage\platforms()
    freePlatform(*stage\platforms())
  Next
  FreeList(*stage\platforms())
  
  freeAnimation(*stage\backgroundAnim)
  FreeStructure(*stage\backgroundAnim)
  FreeStructure(*stage)
EndProcedure

Procedure freeFighter(*fighter.Fighter)
  ForEach *fighter\animations()
    freeAnimation(*fighter\animations())
  Next
  FreeMap(*fighter\animations())
  
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
; CursorPosition = 27
; Folding = --
; EnableXP