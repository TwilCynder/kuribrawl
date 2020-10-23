;trucs pas finis
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

XIncludeFile "utilCore.pb"
XIncludeFile "filelib.pb"
XIncludeFile "gameDataLib.pbi"
XIncludeFile "loadlib.pb"
XIncludeFile "gamelib.pbi"
XIncludeFile "cleanseLib.pbi"
XIncludeFile "physicslib.pb"
XIncludeFile "inputlib.pb"
XIncludeFile "statelib.pb"
XIncludeFile "gameData.pb"
XIncludeFile "window.pb"

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
  
  setStage(*game, getStage("Snowdin"))
  *f1.Fighter = newFighter(*game, getCharacter("Acid"), *game\currentStage\model\w / 2 -200, 300)
  *f1\name = "Test One"
  *f2.Fighter = newFighter(*game, getCharacter("Acid"), *game\currentStage\model\w / 2 +200, 300)
  *f2\name = "Test Two"
  
  ;setPort(0, 0)
  ;setPortFighter(0, *f1)
  ;setPort(1, 3)
  ;setPortFighter(1, *f2)
EndProcedure  

initWindow()

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

Define nextFrame.f, frameWait.f, startTime.l, endTime.l, lastFrameDuration.l, currentTime.l, launchTime.l
updateFrameRate()
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
  updateAnimations(*game)
  renderFrame(*game)
  advanceAnimations(*game)
  SetWindowTitle(window, Str(lastFrameDuration))
  
  currentTime = ElapsedMilliseconds()
  frameWait = nextFrame - currentTime
   If frameWait < 0
     nextFrame = currentTime
     Debug "/!\Can't keep up ! Frame lasted " + Str(frameDuration - frameWait)
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
  
  
; IDE Options = PureBasic 5.31 (Windows - x64)
; CursorPosition = 101
; FirstLine = 43
; Folding = -
; EnableUnicode
; EnableXP