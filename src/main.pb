;TODO : régler finalement la question du : substructure directement en field ou pointer vers heap
;trucs pas finis
;animations des stages/plateformes
;double jump


;TODO list
;finir attack cancel
;créations des stages (dans main.pb -> dans data.twl)
;mouvements verticaux de la caméra

#DEBUG = 1

XIncludeFile "utilCore.pb"
XIncludeFile "filelib.pb"
XIncludeFile "gameDataLib.pbi"
XIncludeFile "loadlib.pb"
XIncludeFile "gamelib.pbi"
XIncludeFile "physicslib.pb"
XIncludeFile "inputlib.pb"
XIncludeFile "statelib.pb"
XIncludeFile "gameData.pb"

CompilerIf #DEBUG
  XIncludeFile "debug.pb"
CompilerEndIf

Define *game.Game, window.l

Define fbf.b

Procedure startTestGame()
  Shared *game, window
  
  If *game
    Debug *game
    endGame(*game)
  EndIf 
  
  *game = initGame(window)
  
  setStage(*game, getStage("Snowdin"))
  *f1.Fighter = newFighter(*game, getCharacter("Acid"), *game\currentStage\model\w / 2 -200, 200)
  *f1\name = "Test One"
  *f2.Fighter = newFighter(*game, getCharacter("Acid"), *game\currentStage\model\w / 2 +200, 200)
  *f2\name = "Test Two"
  
  setPort(0, 3)
  setPortFighter(0, *f1)
  setPort(1, 0)
  setPortFighter(1, *f2)
EndProcedure  

;- Game window
window = OpenWindow(-1, 0, 0, #SCREEN_W + 170, #SCREEN_H, "test", #PB_Window_SystemMenu | #PB_Window_ScreenCentered)
OpenWindowedScreen(WindowID(window), 0, 0, #SCREEN_W, #SCREEN_H, 0, 0, 0, #PB_Screen_NoSynchronization)
SetWindowColor(window, $aaaaaa)
CompilerIf #DEBUG
  InputLogGadget(#SCREEN_W + 5, 5, 160, #SCREEN_H - 120)
  StatesGadgets(#SCREEN_W + 5, #SCREEN_H - 100, 140)
CompilerEndIf 
  CreateMenu(0, WindowID(window))
  AddKeyboardShortcut(window, #PB_Shortcut_F5, 0)
  BindMenuEvent(0, 0, @startTestGame())

;- game data
CompilerIf #DEBUG
  loadGameData("res/data.twl")
  For i = 1 To availableJosticks
    Debug Str(i - 1) + JoystickName(i - 1)
  Next
CompilerElse 
  loadGameData("data.twl")
CompilerEndIf

ForEach kuribrawl\characters()
  initDefaultAnimationsConfig(kuribrawl\characters())
Next 


startTestGame()

;- Main loop (game)

Define nextFrame.f, frameDuration.f, frameWait.f, startTime.l, endTime.l, lastFrameDuration.l, currentTime.l, launchTime.l
frameDuration.f = 1000.0 / 10
nextFrame.f = ElapsedMilliseconds()
endTime = ElapsedMilliseconds()
launchTime = nextFrame
lastFrameDuration = 16
initFighters(*game)
Repeat
  startTime = endTime
  nextFrame = nextFrame + frameDuration
  startTime = ElapsedMilliseconds()
  manageHitboxes(*game)
  updateInputs()
  manageStates(*game)
  applyPhysics(*game)
  advanceAnimations(*game)
  updateAnimations(*game)
  renderFrame(*game)
  SetWindowTitle(window, Str(lastFrameDuration))
  
  currentTime = ElapsedMilliseconds()
  frameWait = nextFrame - currentTime
   If frameWait < 0
     nextFrame = currentTime
     Debug "/!\Can't keep up !"
   Else 
     CompilerIf Not #DEBUG
       If frameWait < 10
         bgc = #Red 
         ;Debug "/!\Frame process lasted more than 2.6ms !"
       EndIf 
     CompilerEndIf
     Delay(frameWait)
   EndIf 
  frame + 1
  endTime = ElapsedMilliseconds()

  lastFrameDuration = endTime - startTime
Until WindowEvent() = #PB_Event_CloseWindow
  
  
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 85
; FirstLine = 36
; Folding = -
; EnableXP
; EnableUnicode