XIncludeFile "utilCore.pb"
XIncludeFile "gamelib.pbi"
XIncludeFile "inputlib.pb"
XIncludeFile "physicslib.pb"

InitSprite()
UsePNGImageDecoder()


window.l = OpenWindow(-1, 0, 0, #SCREEN_W, #SCREEN_H, "test", #PB_Window_SystemMenu | #PB_Window_ScreenCentered)
OpenWindowedScreen(WindowID(window), 0, 0, #SCREEN_W, #SCREEN_H, 0, 0, 0, #PB_Screen_NoSynchronization   )
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
frameDuration.f = 1000.0 / 30
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
  
  SetWindowTitle(window, Str(1000 / lastFrameDuration))
  
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
; CursorPosition = 32
; FirstLine = 6
; EnableXP
; EnableUnicode