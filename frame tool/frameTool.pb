#CANVAS_W = 200
#CANVAS_H = 200

#DEBUG = 0
InitSprite()
InitMouse()

XIncludeFile "../src/utilCore.pb"
XIncludeFile "../src/filelib.pb"
XIncludeFile "../src/gameDatalib.pbi"
XIncludeFile "../src/loadlib.pb"

Procedure defaultJumpAnimCallback(*fighter, *data)
EndProcedure

Procedure defaultAttackAnimCallback(*fighter, *data)
EndProcedure
  

Define *selectedAnim.AnimationModel, *selectedCollisionBox.CollisionBox, viewPosition.Vector

OpenWindow(0, 0, 0, 400, 250, "Kuribrawl Frame Tool", #PB_Window_ScreenCentered | #PB_Window_SystemMenu)
OpenWindowedScreen(WindowID(0), 5, 5, #CANVAS_W, #CANVAS_H)
SpinGadget(0, #CANVAS_W + 10, 5, 20, 50, 0, 1, #PB_Spin_ReadOnly)
TextGadget(1, #CANVAS_W + 10, 60, 10, 20, "x")
SpinGadget(2, #CANVAS_W + 25, 60, 45, 20, -#CANVAS_W / 2, #CANVAS_W / 2, #PB_Spin_Numeric)
TextGadget(3, #CANVAS_W + 80, 60, 10, 20, "y")
SpinGadget(4, #CANVAS_W + 95, 60, 45, 20, -#CANVAS_H / 2, #CANVAS_H / 2, #PB_Spin_Numeric)
TextGadget(5, #CANVAS_W + 10, 90, 10, 20, "w")
SpinGadget(6, #CANVAS_W + 25, 90, 45, 20, -#CANVAS_W / 2, #CANVAS_W / 2, #PB_Spin_Numeric)
TextGadget(7, #CANVAS_W + 80, 90, 10, 20, "h")
SpinGadget(8, #CANVAS_W + 95, 90, 45, 20, -#CANVAS_H / 2, #CANVAS_H / 2, #PB_Spin_Numeric)
ButtonGadget(9, #CANVAS_W + 10, 120, 20, 20, "OK")
StringGadget(10, #CANVAS_W + 35, 120, 150, 20, "")

IncludeFile "../src/gameData.pb"
loadGameData("../src/res/data.twl")

totalAnims.l = 0

ForEach kuribrawl\characters()
  totalAnims + MapSize(kuribrawl\characters()\animations())
Next    
Dim *itemAnims.AnimationModel(totalAnims)
NewMap characters.Champion()


ForEach kuribrawl\characters()
  ForEach kuribrawl\characters()\animations()
    CopyList(kuribrawl\characters()\animations()\frames(), characters(MapKey(kuribrawl\characters()))\animations(MapKey(kuribrawl\characters()\animations()))\frames())
    characters()\animations()\spriteSheet = kuribrawl\characters()\animations()\spriteSheet.l
    characters()\animations()\spriteSheetL = kuribrawl\characters()\animations()\spriteSheetL.l
    characters()\animations()\baseSpeed = kuribrawl\characters()\animations()\baseSpeed.d
  Next 
  characters()\name = kuribrawl\characters()\name
Next

ImportC "user32.lib" ;importing the msvcrt lib, granting access to the windows API
  GetCursorPos_(*p.Point)
EndImport

Procedure drawFrame(*animation.AnimationModel, facing)
  Shared viewPosition, *selectedCollisionBox
  ClearScreen($777777)
  Define spriteSheet.l, x.l, y.l
  If facing = -1 And *animation\spriteSheetL
    spriteSheet = *animation\spriteSheetL
  Else
    spriteSheet = *animation\spriteSheet
  EndIf 
  With *animation\frames()
    ClipSprite(spriteSheet, \display\x, \display\y, \display\w, \display\h)
    x = viewPosition\x
    y = viewPosition\y

    DisplayTransparentSprite(spriteSheet, x, y)
    
    StartDrawing(ScreenOutput())
    Line(viewPosition\x + \origin\x, 0, 1, #CANVAS_H, #Blue)
    Line(0, viewPosition\y + \origin\y, #CANVAS_W, 1, #Blue)
    DrawingMode(#PB_2DDrawing_Outlined)
    ForEach \hitboxes()
      If *selectedCollisionBox = @\hitboxes()
        color.l = $ff00aa
      Else
        color.l = #Red
      EndIf 
      Box(\hitboxes()\x + x + \origin\x, \hitboxes()\y + y + \origin\y, \hitboxes()\x2, \hitboxes()\y2, color)
    Next 
    StopDrawing()
  EndWith
  FlipBuffers()
EndProcedure

Procedure onFrameChanged(*anim.AnimationModel)
  Shared viewPosition
  viewPosition\x = (#CANVAS_W / 2) - (*anim\frames()\display\w / 2) 
  viewPosition\y = (#CANVAS_H / 2) - (*anim\frames()\display\h / 2) 
  drawFrame(*anim, 1)
EndProcedure  


Procedure selectCollisionBox(*b.CollisionBox)
  Shared *selectedAnim, *selectedCollisionBox
  SetGadgetState(2, *b\x)
  SetGadgetState(4, *b\y)
  SetGadgetState(6, *b\x2)
  SetGadgetState(8, *b\y2)
  *selectedCollisionBox = *b
  drawFrame(*selectedAnim, 1)
EndProcedure

Procedure setAnimation(*animation.AnimationModel)
  Shared *selectedAnim
  *selectedAnim = *animation
  ResetList(*selectedAnim\frames())
  NextElement(*selectedAnim\frames())
  onFrameChanged(*selectedAnim)
EndProcedure

Procedure menuCallback()
  Shared *itemAnims()
  event.l = EventMenu()
  setAnimation(*itemAnims(event))
EndProcedure  

Procedure clickCallback()
  Shared *selectedAnim, viewPosition
  Define x.l, y.l, hx.l, hy.l
  x = WindowMouseX(0) - 5
  y = WindowMouseY(0) - 5
  
  If *selectedAnim
    ForEach *selectedAnim\frames()\hitboxes()
      With *selectedAnim\frames()\hitboxes()
        hx = \x + viewPosition\x + *selectedAnim\frames()\origin\x
        hy = \y + viewPosition\y + *selectedAnim\frames()\origin\y
        If x > hx And x < hx + \x2 And y > hy And y < hy + \y2
          selectCollisionBox(@*selectedAnim\frames()\hitboxes())
        EndIf 
      EndWith
    Next
  EndIf 
EndProcedure

Procedure makeHitboxText()
  Shared *selectedCollisionBox, *selectedAnim
  Define text.s
  If Not *selectedCollisionBox
    ProcedureReturn 0
  EndIf 
  text + "h"
  text + Str(ListIndex(*selectedAnim\frames()))
  text + " "
  text + Str(*selectedCollisionBox\x) + " "
  text + Str(*selectedCollisionBox\y) + " "
  text + Str(*selectedCollisionBox\x2) + " "
  text + Str(*selectedCollisionBox\y2) + " "
  SetGadgetText(10, text)
EndProcedure

Procedure gadgetCallback()
  Shared *selectedAnim, *selectedCollisionBox
  gadget = EventGadget()
  event.l = EventType()
  Select gadget
    Case 0
      If Not *selectedAnim
        ProcedureReturn
      EndIf 
      If event = #PB_EventType_Up
        If NextElement(*selectedAnim\frames())
          onFrameChanged(*selectedAnim)
        EndIf 
      Else
        If PreviousElement(*selectedAnim\frames())
          onFrameChanged(*selectedAnim)
        EndIf 
      EndIf 
    Case 2
      If *selectedCollisionBox
        If event = #PB_EventType_Up
          *selectedCollisionBox\x + 1
        ElseIf event = #PB_EventType_Down
          *selectedCollisionBox\x - 1
        EndIf 
        drawFrame(*selectedAnim, 1)
      EndIf
    Case 4
      If *selectedCollisionBox
        If event = #PB_EventType_Up
          *selectedCollisionBox\y - 1
        ElseIf event = #PB_EventType_Down
          *selectedCollisionBox\y + 1
        EndIf
        drawFrame(*selectedAnim, 1)
        
      EndIf
    Case 6
      If *selectedCollisionBox
        If event = #PB_EventType_Up
          *selectedCollisionBox\x2 + 1
        ElseIf event = #PB_EventType_Down
          *selectedCollisionBox\x2 - 1
        EndIf 
        drawFrame(*selectedAnim, 1)
      EndIf
    Case 8
      If *selectedCollisionBox
        If event = #PB_EventType_Up
          *selectedCollisionBox\y2 + 1
        ElseIf event = #PB_EventType_Down
          *selectedCollisionBox\y2 - 1
        EndIf 
        drawFrame(*selectedAnim, 1)
      EndIf
    Case 9
      makeHitboxText()
  EndSelect
EndProcedure

CreateMenu(0, WindowID(0))
MenuTitle("Animation")
OpenSubMenu("Open")
BindEvent(#PB_Event_Menu, @menuCallback())
BindEvent(#PB_Event_LeftClick, @clickCallback())
BindEvent(#PB_Event_Gadget, @gadgetCallback())

Define i.l
ForEach characters()
  OpenSubMenu(characters()\name)
  ForEach characters()\animations()
    MenuItem(i, MapKey(characters()\animations()))
    *itemAnims(i) = @characters()\animations()
    i + 1
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
; CursorPosition = 150
; FirstLine = 136
; Folding = --
; EnableXP