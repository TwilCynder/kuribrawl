Structure App
  *currentGame.Game
  *currentMenu.Menu
  window.i
  frameRate.l
  frameDuration.f
EndStructure

Procedure startMenu(*app.App, *menu.Menu)
  *app\currentMenu = *menu
  If *menu\init
    *menu\init(*menu, *menu\data)
  EndIf 
  *menu\selectedElement = *menu\defaultElement
  *menu\app = *app
EndProcedure

Procedure setCurrentGame(*app.App, *game)
  *app\currentGame = *game
EndProcedure

Procedure startGameApp(*app.App, stage.s)
  *game.Game = startGame(stage)
  setCurrentGame(*app, *game)
  ProcedureReturn *game
EndProcedure

Procedure updateFrameRate(*app.App)
  *app\frameDuration = 1000 / *app\frameRate
  Debug "New framerate " + Str(*app\frameRate)
EndProcedure  
  
Procedure lowerFrameRate(*app.App)
  *app\frameRate - 5
  updateFrameRate(*app)
EndProcedure
  
Procedure increaseFrameRate(*app.App)
  *app\frameRate + 5
  updateFrameRate(*app)
EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 14
; Folding = --
; EnableXP