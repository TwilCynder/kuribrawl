XIncludeFile "utilCore.pb"
XIncludeFile "gamelib.pbi"
XIncludeFile "inputlib.pb"
XIncludeFile "physicslib.pb"

InitSprite()
UsePNGImageDecoder()

window.l = OpenWindow(-1, 0, 0, #SCREEN_W, #SCREEN_H, "test", #PB_Window_SystemMenu | #PB_Window_ScreenCentered)
OpenWindowedScreen(WindowID(window), 0, 0, 940, 540, 0, 0, 0, #PB_Screen_NoSynchronization   )
*game.Game = initGame(window)
*f1.Fighter = newFighter(*game, 48, 500)
*f2.Fighter = newFighter(*game, 160, 0)
*f1\name = "Test One"
*testAnim.Animation = newAnimation(*f1, "idle", "res/idle_strip4.png", 1, 0.05)
addFrame(*testAnim, 0, 0, 34, 76, 17, 76)
addFrame(*testAnim, 34, 0, 34, 76, 17, 76)
addFrame(*testAnim, 68, 0, 34, 76, 17, 76)
addFrame(*testAnim, 102, 0, 34, 76, 17, 76)



resetAnimation(*testAnim)

 *f2\name = "Test Two"
 *testAnim = newAnimation(*f2, "idle", "res/test.png", 1)
 addFrame(*testAnim, 0, 0, 64, 64, 32, 32)
 addFrame(*testAnim, 64, 0, 64, 64, 32, 32)
 addFrame(*testAnim, 128, 0, 64, 64, 32, 32)
 addFrame(*testAnim, 192, 0, 64, 64, 32, 32)
 addFrame(*testAnim, 256, 0, 64, 64, 32, 32)
 addFrame(*testAnim, 320, 0, 64, 64, 32, 32)
 addFrame(*testAnim, 384, 0, 64, 64, 32, 32)
 addFrame(*testAnim, 448, 0, 64, 64, 32, 32)
 resetAnimation(*testAnim)

For i = 1 To availableJosticks
  Debug Str(i - 1) + JoystickName(i - 1)
Next

setPort(0, 0)
setPortFighter(0, *f1)
 setPort(1, 1)
 setPortFighter(1, *f2)


Define nextFrame.f, frameDuration.f, frameWait.f, startTime.l, endTime.l, lastFrameDuration.l, currentTime.l
frameDuration.f = 1000.0 / 60
nextFrame.f = ElapsedMilliseconds()
endTime = ElapsedMilliseconds()
Repeat
  startTime = endTime
  nextFrame = nextFrame + frameDuration
  startTime = ElapsedMilliseconds()
  renderFrame(*game)
  updateInputs()
  NextFrame(*game)
  applyPhysics(*game)
  SetWindowTitle(window, Str(lastFrameDuration))
  
  currentTime = ElapsedMilliseconds()
  frameWait = nextFrame - currentTime
   If frameWait < 0
     nextFrame = currentTime
   Else 
     Delay(frameWait)
   EndIf 
  frame + 1
  endTime = ElapsedMilliseconds()

  lastFrameDuration = endTime - startTime
  
Until WindowEvent() = #PB_Event_CloseWindow
  
  
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 11
; EnableXP
; EnableUnicode