#DEBUG = 1

;améliorer l'interaction entre durée de state et vitesse d'animation

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

;- Game window
window.l = OpenWindow(-1, 0, 0, #SCREEN_W + 170, #SCREEN_H, "test", #PB_Window_SystemMenu | #PB_Window_ScreenCentered)
OpenWindowedScreen(WindowID(window), 0, 0, #SCREEN_W, #SCREEN_H, 0, 0, 0, #PB_Screen_NoSynchronization)
SetWindowColor(window, $aaaaaa)
CompilerIf #DEBUG
  InputLogGadget(#SCREEN_W + 5, 5, 160, #SCREEN_H - 120)
  StatesGadgets(#SCREEN_W + 5, #SCREEN_H - 100, 140)
CompilerEndIf 

;- Game data

XIncludeFile "gameData.pb"

CompilerIf #DEBUG
  loadGameData("res/data.twl")
CompilerElse 
  loadGameData("data.twl")
CompilerEndIf
initDefaultAnimationsConfig(*c1)

;- Test game (game, fighters, ports)
*game.Game = initGame(window)

*f1.Fighter = newFighter(*game, getCharacter("Acid"), 150, 500)
*f1\name = "Test One"
*f2.Fighter = newFighter(*game, getCharacter("Acid"), 500, 500)
*f2\name = "Test Two"
setStage(*game, getStage("FD"))

For i = 1 To availableJosticks
  Debug Str(i - 1) + JoystickName(i - 1)
Next

setPort(0, 2)
setPortFighter(0, *f1)
setPort(1, 1)
setPortFighter(1, *f2)

;- Main loop (game)

Define nextFrame.f, frameDuration.f, frameWait.f, startTime.l, endTime.l, lastFrameDuration.l, currentTime.l, launchTime.l
frameDuration.f = 1000.0 / 30
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
     If frameWait < 10
       ;Debug "/!\Frame process lasted more than 2.6ms !"
     EndIf 
     Delay(frameWait)
   EndIf 
  frame + 1
  endTime = ElapsedMilliseconds()

  lastFrameDuration = endTime - startTime
Until WindowEvent() = #PB_Event_CloseWindow
  
  
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 50
; FirstLine = 12
; Folding = -
; EnableXP
; EnableUnicode