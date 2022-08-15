Define window.l

CompilerIf #DEBUG
  Declare InputLogGadget(x.l, y.l, w.l, h.l)
  Declare StatesGadgets(x.l, y.l, w.l)
CompilerEndIf 
Declare startTestGame()

Procedure resizeCallback()
  Shared window
  Debug WindowWidth(window)
  Debug WindowHeight(window)
EndProcedure


Procedure initWindow()
  Shared window
  ;- Game window
  window = OpenWindow(-1, 0, 0, #SCREEN_W + 170, #SCREEN_H, "test", #PB_Window_SystemMenu | #PB_Window_ScreenCentered | #PB_Window_SizeGadget)
  OpenWindowedScreen(WindowID(window), 0, 0, #SCREEN_W, #SCREEN_H, 0, 0, 0, #PB_Screen_NoSynchronization)
  SetWindowColor(window, $aaaaaa)
  CompilerIf #DEBUG
    InputLogGadget(#SCREEN_W + 5, 5, 160, #SCREEN_H - 120)
    StatesGadgets(#SCREEN_W + 5, #SCREEN_H - 100, 140)
  CompilerEndIf 
  CreateMenu(0, WindowID(window))
  AddKeyboardShortcut(window, #PB_Shortcut_F5, 0)
  BindMenuEvent(0, 0, @startTestGame())
  AddKeyboardShortcut(window, #PB_Shortcut_F6, 1)
  BindMenuEvent(0, 1, @lowerFrameRate())
  AddKeyboardShortcut(window, #PB_Shortcut_F7, 2)
  BindMenuEvent(0, 2, @increaseFrameRate())
  
  BindEvent(#PB_Event_SizeWindow, @resizeCallback(), window)
EndProcedure
; IDE Options = PureBasic 5.31 (Windows - x64)
; CursorPosition = 33
; Folding = -
; EnableUnicode
; EnableXP