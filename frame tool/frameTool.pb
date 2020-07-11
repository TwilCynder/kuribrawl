#DEBUG = 0
InitSprite()

XIncludeFile "../src/utilCore.pb"
XIncludeFile "../src/filelib.pb"
XIncludeFile "../src/gamelib.pbi"
XIncludeFile "../src/loadlib.pb"


OpenWindow(0, 0, 0, 400, 250, "Kuribrawl Frame Tool", #PB_Window_ScreenCentered | #PB_Window_SystemMenu)
OpenWindowedScreen(WindowID(0), 5, 5, 200, 200)
CreateMenu(0, WindowID(0))
MenuTitle("Animation")
OpenSubMenu("Open")

IncludeFile "../src/gameData.pb"
loadGameData("../src/res/data.twl")

ForEach kuribrawl\characters()
  OpenSubMenu(kuribrawl\characters()\name)
  ForEach kuribrawl\characters()\animations()
    MenuItem(-1, MapKey(kuribrawl\characters()\animations()))
  Next 
  CloseSubMenu()
Next   

Define event.l
Repeat
  event = WaitWindowEvent()
  Select event
  EndSelect
  
  ClearScreen(#White)
  
  Delay(16)
Until event = #PB_Event_CloseWindow

; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 23
; EnableXP