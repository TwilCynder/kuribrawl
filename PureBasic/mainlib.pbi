Structure MainData
  *currentGame.Game
  *currentMenu.Menu
EndStructure

Procedure startMenu(*main.MainData, *menu.Menu)
  *main\currentMenu = *menu
  If *menu\init
    *menu\init(*menu, *menu\data)
  EndIf 
  *menu\selectedElement = *menu\defaultElement
EndProcedure

Procedure setCurrentGame(*main.MainData, *game)
  *main\currentGame = *game
EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 10
; Folding = -
; EnableXP