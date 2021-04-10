Procedure lowerFrameRateKey()
  *app = GetWindowData(EventWindow())
  lowerFrameRate(*app)
EndProcedure

Procedure increaseFrameRateKey()
  *app = GetWindowData(EventWindow())
  increaseFrameRate(*app)
EndProcedure

Procedure createAppWindow(*app.App)
 *app\window = OpenWindow(-1, 0, 0, #SCREEN_W + 210, #SCREEN_H, "test", #PB_Window_SystemMenu | #PB_Window_ScreenCentered)
  OpenWindowedScreen(WindowID(*app\window), 0, 0, #SCREEN_W, #SCREEN_H, 0, 0, 0, #PB_Screen_NoSynchronization)
  SetWindowColor(*app\window, $aaaaaa)
  CompilerIf #DEBUG
    InputLogGadget(#SCREEN_W + 5, 5, 200, #SCREEN_H - 120)
    StatesGadgets(#SCREEN_W + 5, #SCREEN_H - 100, 180)
  CompilerEndIf 
    CreateMenu(0, WindowID(*app\window))
    ;AddKeyboardShortcut(*app\window, #PB_Shortcut_F5, 0)
    ;BindMenuEvent(0, 0, @startTestGame())
    AddKeyboardShortcut(*app\window, #PB_Shortcut_F6, 1)
    BindMenuEvent(0, 1, @lowerFrameRate())
    AddKeyboardShortcut(*app\window, #PB_Shortcut_F7, 2)
    BindMenuEvent(0, 2, @increaseFrameRate())
  SetWindowData(*app\window, *app)
EndProcedure

Procedure startMainLoop(*app.App)
  Define *game.Game = *app\currentGame
  Define nextFrame.f, frameWait.f, startTime.l, endTime.l, lastFrameDuration.l, currentTime.l, launchTime.l, totalFrameWait.q
  updateFrameRate(*app)
  
  nextFrame.f = ElapsedMilliseconds()
  endTime = ElapsedMilliseconds()
  launchTime = nextFrame
  lastFrameDuration = 0
  Repeat
    startTime = endTime
    nextFrame = nextFrame + *app\frameDuration
    startTime = ElapsedMilliseconds()
    
    readInputs(*app)
    If *game
      manageStates(*game)
      updateInputs(*game)  
      manageHitboxes(*game)
      applyPhysics(*game)
      updateAnimations(*game)
      renderFrame(*game)
      advanceAnimations(*game)
    ElseIf *app\currentMenu
      renderMenu(*app\currentMenu)
    EndIf 
    
    SetWindowTitle(*app\window, Str(lastFrameDuration))
    
    currentTime = ElapsedMilliseconds()
    frameWait = nextFrame - currentTime
    totalFrameWait + frameWait
     If frameWait < 0
       nextFrame = currentTime
       Debug "/!\Can't keep up !"
     Else 
       CompilerIf Not #DEBUG
         If frameWait < 8
           bgc = #Red 
           Debug "/!\Frame process lasted more than 6.6ms !"
         EndIf 
       CompilerEndIf
       Delay(frameWait)
     EndIf 
    frame + 1
    endTime = ElapsedMilliseconds()
  
    lastFrameDuration = endTime - startTime
  Until WindowEvent() = #PB_Event_CloseWindow
  
  CreateFile(0, "kuribrawl log.txt")
  totalTime = ElapsedMilliseconds() - launchTime
  WriteString(0, "Execution lasted " + Str(totalTime) + "ms  and " + Str(frame) + " frames were displayed (average framewait : " + Str(totalFrameWait / frame) + ").")
  CloseFile(0)
EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 26
; Folding = --
; EnableXP