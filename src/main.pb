;TODO : régler finalement la question du : substructure directement en field ou pointer vers heap

#DEBUG = 0

XIncludeFile "utilCore.pb"
XIncludeFile "filelib.pb"
XIncludeFile "gameDataLib.pbi"
XIncludeFile "loadlib.pb"
XIncludeFile "gamelib.pbi"
XIncludeFile "physicslib.pb"
XIncludeFile "inputlib.pb"
XIncludeFile "statelib.pb"

CompilerIf #DEBUG
  XIncludeFile "debug.pb"
CompilerEndIf

Define *game.Game, window.l

Procedure startTestGame()
  Shared *game, window
  
  If *game
    Debug *game
    endGame(*game)
  EndIf 
  
  *game = initGame(window)

  *f1.Fighter = newFighter(*game, getCharacter("Acid"), 150, 500)
  *f1\name = "Test One"
  *f2.Fighter = newFighter(*game, getCharacter("Acid"), 500, 500)
  *f2\name = "Test Two"
  setStage(*game, getStage("Snowdin"))
  
  DisplayTransparentSprite(*game\currentStage\backgroundAnim\model\spriteSheet, 0, 0)
  FlipBuffers()
  
  
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

;- Game data

XIncludeFile "gameData.pb"

CompilerIf #DEBUG
  loadGameData("res/data.twl")
CompilerElse 
  loadGameData("data.twl")
CompilerEndIf
initDefaultAnimationsConfig(*c1)
  
For i = 1 To availableJosticks
  Debug Str(i - 1) + JoystickName(i - 1)
Next

startTestGame()

;- Main loop (game)

Define nextFrame.f, frameDuration.f, frameWait.f, startTime.l, endTime.l, lastFrameDuration.l, currentTime.l, launchTime.l
frameDuration.f = 1000.0 / 60
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
         Debug "/!\Frame process lasted more than 2.6ms !"
       EndIf 
     CompilerEndIf
     Delay(frameWait)
   EndIf 
  frame + 1
  endTime = ElapsedMilliseconds()

  lastFrameDuration = endTime - startTime
Until WindowEvent() = #PB_Event_CloseWindow
  
  
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 77
; FirstLine = 51
; Folding = -
; EnableXP
; EnableUnicode