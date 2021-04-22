;trucs pas finis
;animations des stages/plateformes
;double jump


;TODO list
;créations des stages (dans main.pb -> dans data.twl)
;crouch
;shield

;TODO (non urgent)
; régler finalement la question du : substructure directement en field ou pointer vers heap
; transformer les données sous forme de nombre fragmenté (input) en structures

#DEBUG = 1

Global shieldColor = RGBA(255, 0, 0, 96)
Global testVar.b

CompilerIf #DEBUG
  XIncludeFile "debugH.pbi"
CompilerEndIf

XIncludeFile "utilCore.pb"
XIncludeFile "filelib.pb"
XIncludeFile "menulib.pbi"
XIncludeFile "gameDataLib.pbi"
XIncludeFile "loadlib.pb"
XIncludeFile "animationlib.pbi"
XIncludeFile "gamelib.pbi"
XIncludeFile "applib.pbi"
XIncludeFile "cleanseLib.pbi"
XIncludeFile "physicslib.pb"
XIncludeFile "inputlib.pb"
XIncludeFile "statelib.pb"
XIncludeFile "mainlib.pbi"

XIncludeFile "gameData.pb"
XIncludeFile "menus.pbi"

CompilerIf #DEBUG
  XIncludeFile "debug.pb"
CompilerEndIf

Define app.App

Procedure startTestGame(*app.App)
  *game.Game = *app\currentGame
  
  If *game
    Debug "A game is already running. (game object adress : " + Hex(*game) + ")"
    endGame(*game)
  EndIf 
  
  *game = startGameApp(*app, "Snowdin")
  addFighter(*game, "Acid", *game\currentStage\model\w / 2 -200, 300, 0, "TwilCynder")
  addFighter(*game, "Acid", *game\currentStage\model\w / 2 +200, 300, 1, "Naelink")
  
  initFighters(*game)
EndProcedure 

;- Game window

  
;- loading game data
Define dataFile.s = "data.twl"
CompilerIf #DEBUG
  dataFile = "../res/data.twl"
  For i = 1 To availableJosticks
    Debug Str(i - 1) + JoystickName(i - 1)
  Next
CompilerEndIf

loadGameData(dataFile)

Debug getLoadedSprite("Acid/shield")

;- Initialization
ForEach kuribrawl\characters()
  initChampion(kuribrawl\characters())
Next 
initHUD()
initStartMenu()

;- Starting the game (test)
setPort(0, 0)
setPort(1, 2)

startTestGame(@app)
;startMenu(@app, getMenu("start"))

;- Main loop (game)
startMainLoop(@app)
 
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 71
; FirstLine = 42
; Folding = -
; EnableXP
; EnableUnicode