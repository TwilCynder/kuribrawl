﻿;trucs pas finis
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

XIncludeFile "utilCore.pb"
XIncludeFile "filelib.pb"
XIncludeFile "menulib.pbi"
XIncludeFile "gameDataLib.pbi"
XIncludeFile "loadlib.pb"
XIncludeFile "animationlib.pbi"
XIncludeFile "gamelib.pbi"
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

Define main.MainData, window.l

Procedure startTestGame()
  Shared main, window
  *game.Game = main\currentGame

  If *game
    Debug "A game is already running. (game object adress : " + Hex(*game) + ")"
    endGame(*game)
  EndIf

  setPort(0, 0)
  ;setPort(1, 2)

  *game = startGame(window, "Snowdin")
  addFighter(*game, "Acid", *game\currentStage\model\w / 2 -200, 300, 0, "TwilCynder")
  addFighter(*game, "Acid", *game\currentStage\model\w / 2 +200, 300, 2 , "Naelink")

  initFighters(*game)

  setCurrentGame(@main, *game)
EndProcedure

;- Game window
window = OpenWindow(-1, 0, 0, #SCREEN_W + 210, #SCREEN_H, "test", #PB_Window_SystemMenu | #PB_Window_ScreenCentered)
OpenWindowedScreen(WindowID(window), 0, 0, #SCREEN_W, #SCREEN_H, 0, 0, 0, #PB_Screen_NoSynchronization)
SetWindowColor(window, $aaaaaa)
CompilerIf #DEBUG
  InputLogGadget(#SCREEN_W + 5, 5, 200, #SCREEN_H - 120)
  StatesGadgets(#SCREEN_W + 5, #SCREEN_H - 100, 180)
CompilerEndIf
  CreateMenu(0, WindowID(window))
  AddKeyboardShortcut(window, #PB_Shortcut_F5, 0)
  BindMenuEvent(0, 0, @startTestGame())
  AddKeyboardShortcut(window, #PB_Shortcut_F6, 1)
  BindMenuEvent(0, 1, @lowerFrameRate())
  AddKeyboardShortcut(window, #PB_Shortcut_F7, 2)
  BindMenuEvent(0, 2, @increaseFrameRate())

;- loading game data
CompilerIf #DEBUG
  loadGameData("data.twl")
  For i = 1 To availableJosticks
    Debug Str(i - 1) + JoystickName(i - 1)
  Next
CompilerElse
  loadGameData("data.twl")
CompilerEndIf

;- Initialization
ForEach kuribrawl\characters()
  initChampion(kuribrawl\characters())
Next
initHUD()
initStartMenu()

;- Starting the game (test)
startTestGame()
;startMenu(@main, getMenu("start"))

;- Main loop (game)

*game.Game = main\currentGame
Define nextFrame.f, frameWait.f, startTime.l, endTime.l, lastFrameDuration.l, currentTime.l, launchTime.l
Define totalFrameWait.q
updateFrameRate()
nextFrame.f = ElapsedMilliseconds()
endTime = ElapsedMilliseconds()
launchTime = nextFrame
lastFrameDuration = 16
Repeat
  startTime = endTime
  nextFrame = nextFrame + frameDuration
  startTime = ElapsedMilliseconds()

  readInputs(*game)
  If *game
    manageStates(*game)
    updateInputs(*game)
    manageHitboxes(*game)
    applyPhysics(*game)
    updateAnimations(*game)
    renderFrame(*game)
    advanceAnimations(*game)
  ElseIf main\currentMenu
    renderMenu(main\currentMenu)
  EndIf

  SetWindowTitle(window, Str(lastFrameDuration))

  currentTime = ElapsedMilliseconds()
  frameWait = nextFrame - currentTime
  totalFrameWait + frameWait
   If frameWait < 0
     nextFrame = currentTime
     Debug "/!\Can't keep up ! Frame lasted " + Str(frameDuration - frameWait)
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
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 55
; FirstLine = 39
; Folding = 8
; EnableXP
; EnableUnicode
