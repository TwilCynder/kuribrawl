#DEBUG = 0

;FINIR LES JUMPS (DASH JUMP, BACK JUMP, DOUBLE JUMP)
;SYSTEM DE DEBUG INTERGRE A LA FENETRE
;BINARY DATA (WRITE + READ)

XIncludeFile "utilCore.pb"
XIncludeFile "filelib.pb"
XIncludeFile "gamelib.pbi"
XIncludeFile "inputlib.pb"
XIncludeFile "statelib.pb"
XIncludeFile "physicslib.pb"
CompilerIf #DEBUG
  XIncludeFile "debug.pb"
CompilerEndIf

InitSprite()
UsePNGImageDecoder()


window.l = OpenWindow(-1, 0, 0, #SCREEN_W + 170, #SCREEN_H, "test", #PB_Window_SystemMenu | #PB_Window_ScreenCentered)
OpenWindowedScreen(WindowID(window), 0, 0, #SCREEN_W, #SCREEN_H, 0, 0, 0, #PB_Screen_NoSynchronization )
SetWindowColor(window, $aaaaaa)
CompilerIf #DEBUG
  InputLogGadget(#SCREEN_W + 5, 5, 160, #SCREEN_H - 120)
  StatesGadgets(#SCREEN_W + 5, #SCREEN_H - 100, 140)
CompilerEndIf 


loadGameData()
XIncludeFile "gameData.pb"

*game.Game = initGame(window)
*f1.Fighter = newFighter(*game, getCharacter("Acid Rainbows"), 48, 500)
*f1\name = "Test One"
;*f2.Fighter = newFighter(*game, getCharacter("Acid Rainbows"), 48, 500)
;*f2\name = "Test Two"


For i = 1 To availableJosticks
  Debug Str(i - 1) + JoystickName(i - 1)
Next

setPort(0, 0)
setPortFighter(0, *f1)
;setPort(1, 1)
;setPortFighter(1, *f2)


Define nextFrame.f, frameDuration.f, frameWait.f, startTime.l, endTime.l, lastFrameDuration.l, currentTime.l
frameDuration.f = 1000.0 / 60
nextFrame.f = ElapsedMilliseconds()
endTime = ElapsedMilliseconds()
lastFrameDuration = 16
Repeat
  startTime = endTime
  nextFrame = nextFrame + frameDuration
  startTime = ElapsedMilliseconds()
  NextFrame(*game)
  updateInputs()
  manageStates(*game)
  applyPhysics(*game)
  updateAnimations(*game)
  renderFrame(*game)
  
  SetWindowTitle(window, Str(lastFrameDuration))
  
  currentTime = ElapsedMilliseconds()
  frameWait = nextFrame - currentTime
   If frameWait < 0
     nextFrame = currentTime
     Debug "/!\Can't keep up !"
   Else 
     If frameWait < 14
       ;Debug "/!\Frame process lasted more than 2.6ms !"
     EndIf 
     Delay(frameWait)
   EndIf 
  frame + 1
  endTime = ElapsedMilliseconds()

  lastFrameDuration = endTime - startTime
  
Until WindowEvent() = #PB_Event_CloseWindow
  
  
; IDE Options = PureBasic 5.72 (Windows - x64)
; Folding = -
; EnableXP
; EnableUnicode