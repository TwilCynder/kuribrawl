#DEBUG = 1

;todo : menu option to make the data file
;menu option to save + make + load

CompilerIf #DEBUG
  #DEFAULT_DATAFILE_PATH = "../src/res/data.twl"
CompilerElse
  #DEFAULT_DATAFILE_PATH = "../data.twl"
CompilerEndIf

Enumeration
  #MENUITEM_SAVE
  #MENUITEM_SAVEALL
  #MENUITEM_SETDESCRIPTOR
  #MENUITEM_LOADDATAFILE
  #MENUITEM_LOADPROJECTDB
  #MENUITEM_RELOAD
  #MENUITEM_ADDHITBOX
  #MENUITEM_ADDHURTBOX
  #MENUITEM_DELETEBOX
  #MENUITEM_GENERATEHURTBOXES
  #MENUITEMS
EndEnumeration

#CANVAS_W = 200
#CANVAS_H = 200

Macro rmii
  firstNonAnimItem +
EndMacro

XIncludeFile "../src/utilCore.pb"
XIncludeFile "../src/filelib.pb"
XIncludeFile "../src/gameDatalib.pbi"
XIncludeFile "../src/loadlib.pb"

Procedure defaultJumpAnimCallback(*fighter, *data)
EndProcedure

Procedure defaultAttackAnimCallback(*fighter, *data)
EndProcedure

Structure AnimationDescriptorPair
  *animation
  descriptor.s
EndStructure

Define *selectedAnim.AnimationModel, *selectedCollisionBox.CollisionBox, viewPosition.Vector, selectedCollisionBoxType.b, firstNonAnimItem.l, projectDBLoaded.b, dataFilePath.s
NewMap characters.Champion()

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
    ForEach \hurtboxes()
      If *selectedCollisionBox = @\hurtboxes()
        color.l = $aa00aa
      Else
        color.l = #Green
      EndIf 
      Box(\hurtboxes()\x + x + \origin\x, \hurtboxes()\y + y + \origin\y, \hurtboxes()\x2, \hurtboxes()\y2, color)
    Next 
    StopDrawing()
  EndWith
  FlipBuffers()
EndProcedure

Procedure onFrameChanged(*anim.AnimationModel)
  Shared viewPosition, *selectedCollisionBox
  *selectedCollisionBox = 0
  viewPosition\x = (#CANVAS_W / 2) - (*anim\frames()\display\w / 2) 
  viewPosition\y = (#CANVAS_H / 2) - (*anim\frames()\display\h / 2) 
  drawFrame(*anim, 1)
EndProcedure  


Procedure selectCollisionBox(*b.CollisionBox, type.b)
  Shared *selectedAnim, *selectedCollisionBox
  SetGadgetState(2, *b\x)
  SetGadgetState(4, *b\y)
  SetGadgetState(6, *b\x2)
  SetGadgetState(8, *b\y2)
  selectedCollisionBoxType = type
  *selectedCollisionBox = *b
  drawFrame(*selectedAnim, 1)
EndProcedure

Declare.s findAnimationDescriptor(*animation)
Procedure setAnimation(*animation.AnimationModel)
  Shared *selectedAnim
  *selectedAnim = *animation
  ResetList(*selectedAnim\frames())
  NextElement(*selectedAnim\frames())
  onFrameChanged(*selectedAnim)
  
  SetGadgetText(11, findAnimationDescriptor(*animation))
EndProcedure

Procedure newHitbox()
  Shared *selectedAnim
  If Not *selectedAnim
    ProcedureReturn 0
  EndIf 
  addHitbox(*selectedAnim\frames(), -20, -60, 40, 40)
  drawFrame(*selectedAnim, 1)
EndProcedure

Procedure newHurtbox()
  Shared *selectedAnim
  If Not *selectedAnim
    ProcedureReturn 0
  EndIf 
  addHurtbox(*selectedAnim\frames(), -20, -60, 40, 40)
  drawFrame(*selectedAnim, 1)
EndProcedure

Procedure deleteBox()
  Shared *selectedAnim, *selectedCollisionBox, selectedCollisionBoxType
  If Not *selectedAnim Or Not *selectedCollisionBox
    ProcedureReturn 0
  EndIf 
  If selectedCollisionBoxType = #CBOX_TYPE_HIT
    ChangeCurrentElement(*selectedAnim\frames()\hitboxes(), *selectedCollisionBox)
    DeleteElement(*selectedAnim\frames()\hitboxes())
  Else  
    ChangeCurrentElement(*selectedAnim\frames()\hurtboxes(), *selectedCollisionBox)
    DeleteElement(*selectedAnim\frames()\hurtboxes())
  EndIf 
  drawFrame(*selectedAnim, 1)
EndProcedure

Procedure clickCallback()
  Shared *selectedAnim, viewPosition
  Define x.l, y.l, hx.l, hy.l
  x = WindowMouseX(0) - 5
  y = WindowMouseY(0) - 5
  
  If *selectedAnim
    ForEach *selectedAnim\frames()\hurtboxes()
      With *selectedAnim\frames()\hurtboxes()
        hx = \x + viewPosition\x + *selectedAnim\frames()\origin\x
        hy = \y + viewPosition\y + *selectedAnim\frames()\origin\y
        If x > hx And x < hx + \x2 And y > hy And y < hy + \y2
          selectCollisionBox(@*selectedAnim\frames()\hurtboxes(), #CBOX_TYPE_HURT)
        EndIf 
      EndWith
    Next
    ForEach *selectedAnim\frames()\hitboxes()
      With *selectedAnim\frames()\hitboxes()
        hx = \x + viewPosition\x + *selectedAnim\frames()\origin\x
        hy = \y + viewPosition\y + *selectedAnim\frames()\origin\y
        If x > hx And x < hx + \x2 And y > hy And y < hy + \y2
          selectCollisionBox(@*selectedAnim\frames()\hitboxes(),   #CBOX_TYPE_HIT)
        EndIf 
      EndWith
    Next
  EndIf 
EndProcedure

Procedure makeHitboxText(type.b)
  Shared *selectedCollisionBox, *selectedAnim
  Define text.s
  If Not *selectedCollisionBox
    ProcedureReturn 0
  EndIf 
  If type = #CBOX_TYPE_HIT
    text + "h"
  Else
    text + "c"
  EndIf 
  text + Str(ListIndex(*selectedAnim\frames()))
  text + " "
  text + Str(*selectedCollisionBox\x) + " "
  text + Str(*selectedCollisionBox\y) + " "
  text + Str(*selectedCollisionBox\x2) + " "
  text + Str(*selectedCollisionBox\y2) + " "
  SetGadgetText(10, text)
EndProcedure

Procedure gadgetCallback()
  Shared *selectedAnim, *selectedCollisionBox, selectedCollisionBoxType
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
      makeHitboxText(selectedCollisionBoxType)
  EndSelect
EndProcedure

InitSprite()
InitMouse()

OpenWindow(0, 0, 0, 400, 250, "Kuribrawl Frame Tool", #PB_Window_ScreenCentered | #PB_Window_SystemMenu)
OpenWindowedScreen(WindowID(0), 5, 5, #CANVAS_W, #CANVAS_H)

IncludeFile "../src/gameData.pb"
Dim *itemAnims.AnimationModel(0)

NewList animationDescriptorFiles.AnimationDescriptorPair()

Procedure.s findAnimationDescriptor(*animation)
  Shared animationDescriptorFiles()
  ForEach animationDescriptorFiles()
    If animationDescriptorFiles()\animation = *animation
      ProcedureReturn animationDescriptorFiles()\descriptor
    EndIf
  Next
  ProcedureReturn ""
EndProcedure

Procedure saveAnimationDescriptor(*animation.AnimationModel, path.s)
  Define i.b = 0
  If path = ""
    ProcedureReturn 0
  EndIf 
  CreateFile(0, path)
  Debug *animation
  WriteStringN(0, Str(ListSize(*animation\frames())))
  If Not *animation\baseSpeed = 1
    If *animation\baseSpeed < 1 And Not *animation\baseSpeed = -1
      WriteStringN(0, "s" + StrF(*animation\baseSpeed, 3))
    Else
      WriteStringN(0, "s" + Str(*animation\baseSpeed))
    EndIf 
  EndIf 
  ForEach *animation\frames()
    If *animation\frames()\duration
      WriteStringN(0, "f" + Str(i) + " d" + Str(*animation\frames()\duration))
    EndIf 
    ForEach *animation\frames()\hitboxes()
      WriteStringN(0, "h" + Str(i) + " " + Str(*animation\frames()\hitboxes()\x) + " " + Str(*animation\frames()\hitboxes()\y) + " " + Str(*animation\frames()\hitboxes()\x2) + " " + Str(*animation\frames()\hitboxes()\y2) + " " + Str(*animation\frames()\hitboxes()\damage))
    Next 
    ForEach *animation\frames()\hurtboxes()
      WriteStringN(0, "c" + Str(i) + " " + Str(*animation\frames()\hurtboxes()\x) + " " + Str(*animation\frames()\hurtboxes()\y) + " " + Str(*animation\frames()\hurtboxes()\x2) + " " + Str(*animation\frames()\hurtboxes()\y2))
    Next 
    i + 1
  Next 
  CloseFile(0)
EndProcedure

Procedure saveAll()
  Shared animationDescriptorFiles()
  ForEach animationDescriptorFiles()
    saveAnimationDescriptor(animationDescriptorFiles()\animation, animationDescriptorFiles()\descriptor)
  Next
EndProcedure

Procedure setDescriptorFile(*animation)
  Shared animationDescriptorFiles()
  If findAnimationDescriptor(*animation) = ""
    AddElement(animationDescriptorFiles())
    animationDescriptorFiles()\animation = *animation
  EndIf 
  animationDescriptorFiles()\descriptor = InputRequester("Kuribrawl Frame Tool", "Name/Path of the descriptor file for this animation : ", "")
  SetGadgetText(11, animationDescriptorFiles()\descriptor)
EndProcedure

Procedure generateDefaultHurtboxes(*animation.AnimationModel)
  ForEach *animation\frames()
    addHurtbox(*animation\frames(), -*animation\frames()\origin\x, -*animation\frames()\origin\y, *animation\frames()\display\w, *animation\frames()\display\h)
  Next
  FirstElement(*animation\frames())
  drawFrame(*animation, 1)
EndProcedure

Procedure setProjectDBLoaded(state.b)
  Shared projectDBLoaded, firstNonAnimItem
  projectDBLoaded = state
  DisableMenuItem(0, rmii #MENUITEM_SAVE, 1 - state)
  DisableMenuItem(0, rmii #MENUITEM_SAVEALL, 1 - state)
  DisableMenuItem(0, rmii #MENUITEM_SETDESCRIPTOR, 1 - state)
EndProcedure

Procedure loadProjectDB(path.s)
  Shared animationDescriptorFiles(), characters()
  Define tag.s, infos.s, file.s, *animation
  If Not ReadFile(0, path)
    ProcedureReturn 0
  EndIf 
  ClearList(animationDescriptorFiles())
  While Not Eof(0)
    ReadString(0) ;osef
    tag = ReadString(0)
    infos = StringField(tag, 2, " ")
    tag = StringField(tag, 1, " ")
    If Not StringField(tag, 1, ":") = "A"
      Continue
    EndIf 
    tag = StringField(tag, 2, ":")
    If Right(infos, 4) = ".dat"
      *animation = characters(StringField(tag, 1, "/"))\animations(StringField(tag, 2, "/"))
      AddElement(animationDescriptorFiles())
      animationDescriptorFiles()\animation = *animation
      animationDescriptorFiles()\descriptor = infos
    EndIf  
  Wend 
  CloseFile(0)
  SetCurrentDirectory(GetPathPart(path))
  ProcedureReturn 1 
EndProcedure

Procedure tryLoadProjectDB(path.s)
  loading:
  If loadProjectDB(path)
    ProcedureReturn 1
  EndIf 
  If MessageRequester("Error", "Can't find project_db.txt in this directory. Do you want to find it yourself ?", #PB_MessageRequester_YesNo) = #PB_MessageRequester_Yes
    path.s = OpenFileRequester("Find this project's project_db.txt", GetCurrentDirectory(), "project_db.txt|*.txt", 0)
    If path = ""
      ProcedureReturn 0
    EndIf 
    Goto loading
  Else
    ProcedureReturn 0
  EndIf   
EndProcedure

Procedure makeMenu()
  Shared *itemAnims(), totalAnims, characters(), firstNonAnimItem
  Dim *itemAnims.AnimationModel(totalAnims)
  CreateMenu(0, WindowID(0))
  MenuTitle("Animation")
  OpenSubMenu("Open")
  
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
  CloseSubMenu()
  firstNonAnimItem = i
  
  MenuItem(rmii #MENUITEM_SAVE, "Save")
  MenuItem(rmii #MENUITEM_SAVEALL, "Save All")
  MenuItem(rmii #MENUITEM_SETDESCRIPTOR, "Set descriptor file")
  
  MenuTitle("Project")
  MenuItem(rmii #MENUITEM_LOADPROJECTDB, "Load project_db")
  MenuItem(rmii #MENUITEM_LOADDATAFILE, "Load Data File (data.twl)")
  MenuItem(rmii #MENUITEM_RELOAD, ""
  
  MenuTitle("Cboxes")
  MenuItem(rmii #MENUITEM_ADDHITBOX, "Add hitbox")
  MenuItem(rmii #MENUITEM_ADDHURTBOX, "Add hurtbox")
  MenuItem(rmii #MENUITEM_DELETEBOX, "Delete collision box")
  MenuItem(rmii #MENUITEM_GENERATEHURTBOXES, "Generate default hurtboxes")
EndProcedure  

Procedure onLoad()
  Shared characters(), totalAnims
  
  totalAnims = 0
  ForEach kuribrawl\characters()
    ForEach kuribrawl\characters()\animations()
      CopyList(kuribrawl\characters()\animations()\frames(), characters(MapKey(kuribrawl\characters()))\animations(MapKey(kuribrawl\characters()\animations()))\frames())
      characters()\animations()\spriteSheet = kuribrawl\characters()\animations()\spriteSheet.l
      characters()\animations()\spriteSheetL = kuribrawl\characters()\animations()\spriteSheetL.l
      characters()\animations()\baseSpeed = kuribrawl\characters()\animations()\baseSpeed.d
      totalAnims + 1
    Next 
    characters()\name = kuribrawl\characters()\name
  Next
  makeMenu()
EndProcedure

Procedure load(path.s)
  Shared dataFilePath
  loadDatafile:
  If Not loadGameData(path)
    If MessageRequester("Kuribrawl Frame Tool", "Can't find data file (data.twl). Do you want to look for it maunally ?", #PB_MessageRequester_YesNo) = #PB_MessageRequester_Yes
      path = OpenFileRequester("Find data.twl", "", "Kuribrawl Data File (data.twl) | data.twl", 0)
      If Not path = ""
        Goto loadDataFile
      EndIf 
    EndIf
    ProcedureReturn 0 
  EndIf
  dataFilePath = path
  onLoad()
  ProcedureReturn 1
EndProcedure

Procedure reload()
  Shared characters(), dataFilePath, animationDescriptorFiles()
  ForEach kuribrawl\characters()
    ClearMap(kuribrawl\characters()\animations())
  Next
  ClearMap(characters())
  ClearList(animationDescriptorFiles())
  *selectedAnim = 0
  load(dataFilePath)
  SetGadgetText(11, "")
  tryLoadProjectDB("project_db.txt")
EndProcedure 

Procedure menuCallback()
  Shared *itemAnims(), firstNonAnimItem, *selectedAnim
  event.l = EventMenu()
  If event < firstNonAnimItem
    setAnimation(*itemAnims(event))
  Else
    Select event
      Case rmii #MENUITEM_ADDHITBOX
        newHitbox()
      Case rmii #MENUITEM_ADDHURTBOX
        newHurtbox()
      Case rmii #MENUITEM_DELETEBOX
        deleteBox() 
      Case rmii #MENUITEM_SAVE
        If *selectedAnim 
          saveAnimationDescriptor(*selectedAnim, findAnimationDescriptor(*selectedAnim))
        EndIf     
      Case rmii #MENUITEM_SAVEALL
        saveAll()
      Case rmii #MENUITEM_SETDESCRIPTOR
        If *selectedAnim
          setDescriptorFile(*selectedAnim)
        EndIf 
      Case rmii #MENUITEM_GENERATEHURTBOXES
        If *selectedAnim
          generateDefaultHurtboxes(*selectedAnim)
        EndIf 
      Case rmii #MENUITEM_LOADPROJECTDB
        setProjectDBLoaded(Bool(loadProjectDB(OpenFileRequester("Find this project's project_db.txt", GetCurrentDirectory(), "Text Files (.txt)|*.txt", 0))))
      Case rmii #MENUITEM_LOADDATAFILE
        reload()
      Case rmii #MENUITEM_RELOAD
    EndSelect
  EndIf 
EndProcedure   

BindEvent(#PB_Event_Menu, @menuCallback())
BindEvent(#PB_Event_LeftClick, @clickCallback())
BindEvent(#PB_Event_Gadget, @gadgetCallback())

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
TextGadget(11, 5, #CANVAS_H + 10, #CANVAS_W, 20, "")

If Not load(#DEFAULT_DATAFILE_PATH)
  End
EndIf 

setProjectDBLoaded(tryLoadProjectDB("project_db.txt"))

Define event.l
Repeat
  event = WaitWindowEvent()
  Select event
  EndSelect
  
  ClearScreen(#White)
  
  Delay(16)
Until event = #PB_Event_CloseWindow

; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 474
; FirstLine = 437
; Folding = -----
; EnableXP