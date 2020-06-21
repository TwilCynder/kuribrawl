#DEBUG = #PB_Compiler_Debugger
Enumeration
  #DIRECTION_RIGHT
  #DIRECTION_UP
  #DIRECTION_LEFT
  #DIRECTION_DOWN
  #DIRECTION_NONE
EndEnumeration
Structure Vector
  x.l
  y.l
EndStructure
Structure VectorDouble
  x.d
  y.d
EndStructure
Structure Rect_ Extends Vector
  w.l
  h.l
EndStructure
Structure QueueElem
  *next.QueueElem
  value.l
EndStructure
Structure Queue
  *tail.QueueElem
  *sentinel.QueueElem
EndStructure
Procedure newQueue()
  *q.Queue = AllocateMemory(SizeOf(Queue))
  *q\sentinel = AllocateMemory(SizeOf(Queue))
  *q\sentinel\value = 0
  *q\sentinel\next = *q\sentinel
  *q\tail = *q\sentinel
  ProcedureReturn *q
EndProcedure
Macro QHead
  sentinel\next
EndMacro
Macro QSize
  sentinel\value
EndMacro
Procedure QPush(*q.Queue, value.b)
  *elem.QueueElem = AllocateMemory(SizeOf(QueueElem))
  *elem\value = value
  *q\tail\next = *elem
  *q\tail = *elem
  *elem\next = *q\sentinel
*q\sentinel\value + 1
EndProcedure
Procedure.b QPop(*q.Queue)
res.b = *q\sentinel\next\value
*oldHead = *q\sentinel\next
*q\sentinel\next = *q\sentinel\next\next
*q\sentinel\value - 1
If *q\sentinel\value < 1
    *q\tail = *q\sentinel
  EndIf
  FreeMemory(*oldHead)
  ProcedureReturn res
EndProcedure
Prototype QCallback(value.l)
Procedure QIterate(*q.Queue, callback.QCallback)
  *elem.QueueElem = *q\sentinel
  While Not (*elem\next = *q\sentinel)
    *elem = *elem\next
    callback(*elem\value)
  Wend
EndProcedure
NewMap loadedSprites.l()
Procedure checkSignature()
  If Not ReadInteger(0) = $54545454
    Debug "error"
    End
  EndIf
EndProcedure
Procedure readVersion()
  Define maj.a, min.a, rev.a
  maj = ReadAsciiCharacter(0)
  min = ReadAsciiCharacter(0)
  rev = ReadAsciiCharacter(0)
  Debug maj
  Debug min
  Debug rev
EndProcedure
Procedure readFileType()
  ProcedureReturn ReadAsciiCharacter(0)
EndProcedure
Procedure.s readFileTag()
  ProcedureReturn ReadString(0)
EndProcedure
Procedure readFileLength()
  ProcedureReturn ReadInteger(0)
EndProcedure
Procedure readFileData(size.l, *buffer)
  *buffer = ReAllocateMemory(*buffer, size)
  ReadData(0, *buffer, size)
  ProcedureReturn *buffer
EndProcedure
Procedure checkInterfile()
  If Not ReadAsciiCharacter(0) = $54
    Debug "error"
    End
  EndIf
EndProcedure
Procedure LoadSprite_(*buffer, tag.s)
  Shared loadedSprites()
  loadedSprites(tag) = CatchSprite(#PB_Any, *buffer, #PB_Sprite_AlphaBlending)
EndProcedure
Procedure loadGameData()
  Shared loadedSprites()
  If #DEBUG
    OpenFile(0, "res/data.twl")
  Else 
    OpenFile(0, "data.twl")
  EndIf 
  checkSignature()
  readVersion()
  Define type.b, tag.s, size.l, *buffer
  *buffer = 0
  Repeat
    type = readFileType()
    tag = readFileTag()
    size = readFileLength()
    *buffer = readFileData(size, *buffer)
    LoadSprite_(*buffer, tag)
    checkInterfile()
  Until Eof(0)
  CloseFile(0)
 EndProcedure
frame.l = 0
#SCREEN_W = 960
#SCREEN_H = 540
Enumeration 
  #ACTION_WALK
  #ACTION_DASH
  #ACTION_JUMP
  #ACTION_ATTACK
  #ACTION_NORMAL_ATTACK
  #ACTION_SPECIAL_ATTACK
  #ACTION_SHIELD
  #ACTION_DODGE
  #ACTION_GRAB
  #ACTIONS_TOTAL
EndEnumeration  
Enumeration
  #CBOX_TYPE_HURT
  #CBOX_TYPE_HIT
EndEnumeration
Enumeration
  #CBOX_SHAPE_CIRCLE
  #CBOX_SHAPE_BUBBLE
  #CBOX_SHAPE_SQUARE
  #CBOX_SHAPE_RECT
EndEnumeration
Enumeration
  #STATE_IDLE
  #STATE_CROUCH
  #STATE_WALK
  #STATE_WALK_TURN
  #STATE_DASH_START
  #STATE_DASH
  #STATE_DASH_TURN
  #STATE_DASH_STOP
  #STATE_JUMPSQUAT
  #STATE_LANDING
  #STATE_ATTACK
  #STATE_TUMBLE
  #STATE_LANDING_LAG
  #STATES
EndEnumeration
Enumeration
  #JUMP_NORMAL
  #JUMP_WALKING
  #JUMP_BACKWARDS
EndEnumeration
Enumeration
  #YJUMP_NORMAL
  #YJUMP_SHORT
  #YJUMP_DOUBLE
EndEnumeration
#MAX_BUTTON_NB = 31
#MAX_AXIS_NB = 8
Structure Physics
  v.VectorDouble
  a.VectorDouble
EndStructure
Structure CollisionBox
  x.l
  y.l
  shape.b
  s.l  
  s2.l
  x2.l
  y2.l
EndStructure
Structure Hurtbox Extends CollisionBox
  invincible.b
EndStructure
Structure Hitbox Extends CollisionBox
  damage.l
  hitstunModifier.l
  bkb.d 
  skb.d 
  angle.d
  hit.b
EndStructure  
Structure Frame
  display.Rect_
  origin.Vector
  actionnable.b
  actions.b[#ACTIONS_TOTAL] 
  List *collisionBoxes.CollisionBox()
  *callback.f_callback
  duration.b
EndStructure  
Structure Animation
  spriteSheet.l 
  spriteSheetL.l 
  List frames.Frame()
  frameMultiplier.b
  frameCount.b
EndStructure  
Dim stateDefaultAnimation.s(#STATES)
Structure Champion
  Map animations.Animation()
  name.s
  walkSpeed.d
  dashSpeed.d
  initialDashSpeed.d
  dashTurnAccel.d
  maxAirSpeed.d
  airAcceleration.d
  traction.d
  jumpSpeed.d
  jumpsquatDuration.b
  dashStopDuration.b
  dashStartDuration.b
  dashTurnDuration.b
  shorthopSpeed.d
  doubleJumpSpeed.d
  maxFallSpeed.d
  fastFallSpeed.d
  airFriction.d
EndStructure
Structure GameVariables
  backwardJumpBoost.d
  walkingJumpBoost.d
  doubleJumpBackwardSpeed.d
EndStructure
Structure GameData
  Map characters.Champion()
  variables.GameVariables
EndStructure  
Global kuribrawl.GameData
Structure Fighter
  x.l
  y.l
  physics.Physics
  *character.Champion
  Map animations.Animation()
  *currentAnimation.Animation 
  currentAnimationName.s
  port.b
  name.s
  grounded.b
  facing.b 
  state.b
  stateInfo.b 
  stateTimer.u
  jumps.b
EndStructure
Structure Game
  List fighters.Fighter()
  window.l
EndStructure
Prototype.i f_callback(*fighter.Fighter)
Procedure initGame(window.l)
  *game.Game = AllocateStructure(Game)
  *game\window = window
  ProcedureReturn *game
EndProcedure
Procedure newCharacter(name.s)
  *r = AddMapElement(kuribrawl\characters(), name)
  ProcedureReturn *r
EndProcedure
Procedure getCharacter(name.s)
  ProcedureReturn @kuribrawl\characters(name)
EndProcedure
Procedure newAnimation(*character.Champion, name.s, spriteTag.s, speed.d = 1)
  Shared loadedSprites()
  *character\animations(name)\spriteSheet = loadedSprites(spriteTag)
  *animation.Animation = @*character\animations()
  *animation\frameMultiplier = Int(1 / speed)
  ProcedureReturn *animation
EndProcedure
Procedure addLeftSpritesheet(*animation.Animation, tag.s)
  Shared loadedSprites()
  *animation\spriteSheetL = loadedSprites(tag)
EndProcedure
Procedure resetAnimation(*animation.Animation)
  ResetList(*animation\frames())
  NextElement(*animation\frames())
  If *animation\frameMultiplier > 1
    *animation\frames()\duration = *animation\frameMultiplier
  EndIf 
EndProcedure
Procedure setAnimation(*fighter.Fighter, name.s)
  If *fighter\currentAnimationName = name
    ProcedureReturn 0
  EndIf
  *fighter\currentAnimation = @*fighter\animations(name)
  *fighter\currentAnimationName = name
  resetAnimation(*fighter\currentAnimation)
EndProcedure
Procedure addFrame(*animation.Animation, x.l, y.l, w.l, h.l, xo.l, yo.l)
  *f.Frame = AddElement(*animation\frames())
  *f\display\x = x
  *f\display\y = y
  *f\display\w = w
  *f\display\h = h
  *f\origin\x = xo
  *f\origin\y = yo
  *animation\frameCount + *animation\frameMultiplier
EndProcedure
Procedure newFighter(*game.Game, *character.Champion, x.l, y.l, port = -1)
  Define *anim.Animation
  *r.Fighter = AddElement(*game\fighters())
  *r\x = x
  *r\y = y
  *r\physics\v\x = 0
  *r\physics\v\y = 0
  *r\physics\a\x = 0
  *r\physics\a\y = 0
  *r\character = *character
  ForEach *character\animations()
    *anim = AddMapElement(*r\animations(), MapKey(*character\animations()))
    *anim\frameMultiplier = *character\animations()\frameMultiplier
    *anim\frameCount = *character\animations()\frameCount
    *anim\spriteSheet = *character\animations()\spriteSheet
    *anim\spriteSheetL = *character\animations()\spriteSheetL
    CopyList(*character\animations()\frames(), *anim\frames())
    setAnimation(*r, MapKey(*character\animations()))
  Next 
  *r\state = #STATE_IDLE
  *r\port = port
  ProcedureReturn *r
EndProcedure
Procedure renderFrame(*game.Game)
  Define *fighter.Fighter, y.l, spriteSheet
  ClearScreen(#White)
  ForEach *game\fighters()
    *fighter = @*game\fighters()
    If *fighter\facing = -1 And *fighter\currentAnimation\spriteSheetL
      spriteSheet = *fighter\currentAnimation\spriteSheetL
    Else
      spriteSheet = *fighter\currentAnimation\spriteSheet
    EndIf 
    With *fighter\currentAnimation\frames()
y = #SCREEN_H - *fighter\y - *fighter\currentAnimation\frames()\origin\y
ClipSprite(spriteSheet, *fighter\currentAnimation\frames()\display\x, *fighter\currentAnimation\frames()\display\y, *fighter\currentAnimation\frames()\display\w, *fighter\currentAnimation\frames()\display\h)
DisplayTransparentSprite(spriteSheet, *fighter\x - *fighter\currentAnimation\frames()\origin\x, y)
    EndWith
  Next
  FlipBuffers()
EndProcedure
Procedure NextFrame(*game.Game)
  ForEach *game\fighters()
    If *game\fighters()\currentAnimation\frames()\duration > 1
      *game\fighters()\currentAnimation\frames()\duration - 1
    ElseIf NextElement(*game\fighters()\currentAnimation\frames()) = 0
      resetAnimation(*game\fighters()\currentAnimation)
    ElseIf *game\fighters()\currentAnimation\frameMultiplier > 1
      *game\fighters()\currentAnimation\frames()\duration = *game\fighters()\currentAnimation\frameMultiplier
    EndIf
  Next
EndProcedure
Procedure isFighterGrounded(*fighter.Fighter)
  ProcedureReturn *fighter\grounded
EndProcedure
Procedure figherDirection(*fighter.Fighter)
  ProcedureReturn *fighter\facing
EndProcedure
Procedure setState(*fighter.Fighter, state.b, info.l = 0)
  *fighter\state = state
  *fighter\stateInfo = info
  *fighter\stateTimer = 0
EndProcedure
Procedure jump(*fighter.Fighter, jumpTypeX.b, jumpTypeY.b)
  Select jumpTypeX
    Case #JUMP_WALKING
      *fighter\physics\v\x + (kuribrawl\variables\walkingJumpBoost * *fighter\facing)
    Case #JUMP_BACKWARDS
      If jumpTypeY = #YJUMP_DOUBLE
        *fighter\physics\v\x = (kuribrawl\variables\doubleJumpBackwardSpeed * -*fighter\facing)
      Else  
        *fighter\physics\v\x + (kuribrawl\variables\backwardJumpBoost * -*fighter\facing)
      EndIf 
  EndSelect    
  Select jumpTypeY
    Case #YJUMP_SHORT
      *fighter\physics\v\y = *fighter\character\shorthopSpeed
    Case #YJUMP_NORMAL
      *fighter\physics\v\y = *fighter\character\jumpSpeed
    Case #YJUMP_DOUBLE
      *fighter\physics\v\y = *fighter\character\doubleJumpSpeed
  EndSelect
  setState(*fighter, #STATE_IDLE)
EndProcedure
Procedure updateAnimations(*game.Game)
  Define *fighter.Fighter, animation.s
  Shared stateDefaultAnimation()
  ForEach *game\fighters()
    *fighter = @*game\fighters()
    animation = stateDefaultAnimation(*fighter\state)
    If animation And Not animation = *fighter\currentAnimationName
      setAnimation(*fighter, animation)
    EndIf 
  Next 
EndProcedure
Enumeration 
  #INPUT_Attack
  #INPUT_Special
  #INPUT_Jump
  #INPUT_Grab
  #INPUT_Guard
  #INPUT_Smash
  #INPUT_ControlStick_RIGHT
  #INPUT_ControlStick_UP
  #INPUT_ControlStick_LEFT
  #INPUT_ControlStick_DOWN
  #INPUT_ControlStick_SRIGHT
  #INPUT_ControlStick_SUP
  #INPUT_ControlStick_SLEFT
  #INPUT_ControlStick_SDOWN
  #INPUTS
EndEnumeration
Enumeration 
  #Command_Walk
  #Command_Dash
  #Command_Jump
  #COMMAND_Jab
  #COMMAND_FTilt
  #COMMAND_UTilt
  #COMMAND_DTilt
  #COMMAND_FSmash
  #COMMAND_USmash
  #COMMAND_DSmash
  #COMMAND_DashAttack
  #COMMAND_NAir
  #COMMAND_FAir
  #COMMAND_BAir
  #COMMAND_UAir
  #COMMAND_DAir
  #COMMAND_ZAir
  #COMMAND_NSpecial
  #COMMAND_SSpecial
  #COMMAND_USpecial
  #COMMAND_DSpecial 
  #COMMAND_Grab
  #COMMAND_Shield
  #COMMAND_Roll
  #COMMAND_SpotDodge
  #COMMAND_AirDodge
  #COMMAND_DirectionalAirDodge
EndEnumeration
Enumeration 
  #BUTTON_A
  #BUTTON_B
  #BUTTON_X
  #BUTTON_Y
  #BUTTON_Z
  #BUTTON_R
  #BUTTON_L
  #BUTTON_START
  #BUTTONS
EndEnumeration
Enumeration 
  #AXIS_LEFT
  #AXIS_RIGHT
  #AXISES
EndEnumeration
Structure InputConfig
  analogStickThreshold.l
  analogStickSmashThreshold.l
  analogTriggerThreshold.l
EndStructure
Macro stickTreshold :
 defaultControler\config\analogStickThreshold :
 EndMacro
Macro stickSmashTreshold :
 defaultControler\config\analogStickSmashThreshold :
 EndMacro
Macro triggerTreshold :
 defaultControler\config\analogTriggerThreshold :
 EndMacro
Structure Bind
  ID.b
  input.b
EndStructure
Structure InputBinding
  controlStickID.b
  List buttons.Bind()
  List axises.Bind()
  List triggers.Bind()
EndStructure
Structure knownController
  config.InputConfig
  defaultBinding.InputBinding
EndStructure  
Structure inputData
  element.b 
  stick.b 
EndStructure
Structure AxisState
  x.l
  y.l
  z.l
EndStructure
Structure ControllerState
  buttons.b[#MAX_BUTTON_NB]
  axis.AxisState[#MAX_AXIS_NB]
EndStructure
Structure Port
  joyID.l
  *joyType.knownController
  previousState.ControllerState
  currentControlStickState.AxisState
  controlStickBuffer.b[4]
  active.b
  *figher.Fighter
EndStructure
Dim ports.Port(4)
Dim defaultInputDurability(#INPUTS)
defaultInputDurability(#INPUT_Special) = 0
defaultInputDurability(#INPUT_Attack) = 0
defaultInputDurability(#INPUT_Jump) = 0
defaultInputDurability(#INPUT_Grab) = 0
defaultInputDurability(#INPUT_Guard) = 0
defaultInputDurability(#INPUT_ControlStick_SRIGHT) = 5
defaultInputDurability(#INPUT_ControlStick_SDOWN) = 5
defaultInputDurability(#INPUT_ControlStick_SLEFT) = 5
defaultInputDurability(#INPUT_ControlStick_SUP) = 5
defaultInputDurability(#INPUT_ControlStick_RIGHT) = 0
defaultInputDurability(#INPUT_ControlStick_DOWN) = 0
defaultInputDurability(#INPUT_ControlStick_LEFT) = 0
defaultInputDurability(#INPUT_ControlStick_UP) = 0
Prototype inputManager(*port.Port, *info.inputData)
Dim *inputManagers(#INPUTS)
defaultControler.knownController
defaultControler\config\analogStickThreshold = 300
defaultControler\config\analogStickSmashThreshold = 750
defaultControler\config\analogTriggerThreshold = -500
defaultBind.InputBinding
With defaultControler\defaultBinding
AddElement( defaultControler\defaultBinding\buttons()) :
  defaultControler\defaultBinding\buttons()\ID = #BUTTON_A :
  defaultControler\defaultBinding\buttons()\input = #INPUT_Attack
AddElement( defaultControler\defaultBinding\buttons()) :
  defaultControler\defaultBinding\buttons()\ID = #BUTTON_B :
  defaultControler\defaultBinding\buttons()\input = #INPUT_Special
AddElement( defaultControler\defaultBinding\buttons()) :
  defaultControler\defaultBinding\buttons()\ID = #BUTTON_X :
  defaultControler\defaultBinding\buttons()\input = #INPUT_Jump
AddElement( defaultControler\defaultBinding\buttons()) :
  defaultControler\defaultBinding\buttons()\ID = #BUTTON_Y :
  defaultControler\defaultBinding\buttons()\input = #INPUT_Jump
AddElement( defaultControler\defaultBinding\buttons()) :
  defaultControler\defaultBinding\buttons()\ID = #BUTTON_Z :
  defaultControler\defaultBinding\buttons()\input = #INPUT_Grab
AddElement( defaultControler\defaultBinding\axises()) :
  defaultControler\defaultBinding\axises()\ID = #AXIS_RIGHT :
  defaultControler\defaultBinding\axises()\input = #INPUT_Attack
AddElement( defaultControler\defaultBinding\triggers()) :
  defaultControler\defaultBinding\triggers()\ID = #AXIS_LEFT :
  defaultControler\defaultBinding\triggers()\input = #INPUT_Guard
AddElement( defaultControler\defaultBinding\triggers()) :
  defaultControler\defaultBinding\triggers()\ID = #AXIS_RIGHT :
  defaultControler\defaultBinding\triggers()\input = #INPUT_Guard
 defaultControler\defaultBinding\controlStickID = 0
EndWith
gameCube.knownController
gameCube\config\analogStickThreshold = 300
gameCube\config\analogStickSmashThreshold = 750
gameCube\config\analogTriggerThreshold = -500
With gameCube\defaultBinding
AddElement( gameCube\defaultBinding\buttons()) :
  gameCube\defaultBinding\buttons()\ID = #BUTTON_A :
  gameCube\defaultBinding\buttons()\input = #INPUT_Attack
AddElement( gameCube\defaultBinding\buttons()) :
  gameCube\defaultBinding\buttons()\ID = #BUTTON_B :
  gameCube\defaultBinding\buttons()\input = #INPUT_Special
AddElement( gameCube\defaultBinding\buttons()) :
  gameCube\defaultBinding\buttons()\ID = #BUTTON_X :
  gameCube\defaultBinding\buttons()\input = #INPUT_Jump
AddElement( gameCube\defaultBinding\buttons()) :
  gameCube\defaultBinding\buttons()\ID = #BUTTON_Y :
  gameCube\defaultBinding\buttons()\input = #INPUT_Jump
AddElement( gameCube\defaultBinding\buttons()) :
  gameCube\defaultBinding\buttons()\ID = #BUTTON_Z :
  gameCube\defaultBinding\buttons()\input = #INPUT_Grab
AddElement( gameCube\defaultBinding\axises()) :
  gameCube\defaultBinding\axises()\ID = #AXIS_RIGHT :
  gameCube\defaultBinding\axises()\input = #INPUT_Attack
AddElement( gameCube\defaultBinding\triggers()) :
  gameCube\defaultBinding\triggers()\ID = #AXIS_LEFT :
  gameCube\defaultBinding\triggers()\input = #INPUT_Guard
AddElement( gameCube\defaultBinding\triggers()) :
  gameCube\defaultBinding\triggers()\ID = #AXIS_RIGHT :
  gameCube\defaultBinding\triggers()\input = #INPUT_Guard
 gameCube\defaultBinding\controlStickID = 0
EndWith
NewList inputQ.l()
Procedure setPort(port, joyID, active = 1)
  Shared ports()
  ports(port)\joyID = joyID
  ports(port)\active = active
EndProcedure
Procedure setPortFighter(port, *fighter.Fighter)
  Shared ports()
  ports(port)\figher = *fighter
  *fighter\port = port
EndProcedure
Procedure readButton(button.b, *port.Port) 
  Shared ports()
  ProcedureReturn JoystickButton(*port\joyID, button + 1)
EndProcedure
Procedure readAxis(*state.AxisState, axis.b, *port.Port)
  Shared ports()
  *state\x = JoystickAxisX(*port\joyID, axis, #PB_Relative)
  *state\y = JoystickAxisY(*port\joyID, axis, #PB_Relative)
  *state\z = JoystickAxisZ(*port\joyID, axis, #PB_Relative)
EndProcedure
Procedure stickDirection(*state.AxisState)
  Shared defaultControler
  x.l = *state\x
  y.l = *state\y
If x > defaultControler\config\analogStickThreshold  And x > Abs(y)
    ProcedureReturn #DIRECTION_RIGHT
ElseIf y > defaultControler\config\analogStickThreshold  And y > Abs(x)
    ProcedureReturn #DIRECTION_DOWN
ElseIf x < -defaultControler\config\analogStickThreshold  And -x > Abs(y)
    ProcedureReturn #DIRECTION_LEFT
ElseIf y < -defaultControler\config\analogStickThreshold  And -y > Abs(x)
    ProcedureReturn #DIRECTION_UP
  Else 
    ProcedureReturn #DIRECTION_NONE
  EndIf 
EndProcedure
Procedure controlStickDirection(*port.Port)
  ProcedureReturn stickDirection(*port\currentControlStickState)
EndProcedure
Procedure readTrigger(axis.b, *port.Port) 
  Shared ports()
  res.l = JoystickAxisZ(*port\joyID, axis, #PB_Relative)
  ProcedureReturn res
EndProcedure
Procedure makeInputValue(input.b, time.b, port.b, element.b, stick.b)
  ProcedureReturn input + (time << 5) + (port << 9) + (stick << 12) + (element << 13)
EndProcedure
Procedure registerInput(port, input, element = #MAX_BUTTON_NB, stick = 0)
  Shared inputQ(), defaultInputDurability()
  AddElement(inputQ()) :
 inputQ() = makeInputValue(input, defaultInputDurability(input), port, element, stick)
EndProcedure
Procedure readInputs()
  Shared ports(), defaultControler, *port.Port
  Define state.b, id.b, axisState.AxisState
  For i = 0 To 3
    If Not ports(i)\active
      Continue  
    EndIf 
    *port = @ports(i)
    If Not ExamineJoystick(*port\joyID)
      Continue
    EndIf 
    ForEach defaultControler\defaultBinding\buttons()
      id = defaultControler\defaultBinding\buttons()\ID
      state = readButton(id, *port)
      If state And Not ports(i)\previousState\buttons[id]
        registerInput(i, defaultControler\defaultBinding\buttons()\input, id)
      EndIf
      *port\previousState\buttons[id] = state
    Next 
    id = defaultControler\defaultBinding\controlStickID
    readAxis(*port\currentControlStickState, id, *port)
If *port\currentControlStickState\x > defaultControler\config\analogStickSmashThreshold 
If *port\previousState\axis[id]\x < defaultControler\config\analogStickThreshold  XOr *port\controlStickBuffer[#DIRECTION_RIGHT] = 1
        registerInput(i, #INPUT_ControlStick_SRIGHT)
        registerInput(i, #INPUT_ControlStick_RIGHT)
      EndIf 
      *port\controlStickBuffer[#DIRECTION_RIGHT] = 0
ElseIf *port\currentControlStickState\x > defaultControler\config\analogStickThreshold  And *port\previousState\axis[id]\x < defaultControler\config\analogStickThreshold  And         *port\controlStickBuffer[#DIRECTION_LEFT] = 0
      *port\controlStickBuffer[#DIRECTION_RIGHT] = 1
ElseIf *port\currentControlStickState\x < -defaultControler\config\analogStickSmashThreshold 
If *port\previousState\axis[id]\x > -defaultControler\config\analogStickThreshold  XOr *port\controlStickBuffer[#DIRECTION_LEFT] = 1
        registerInput(i, #INPUT_ControlStick_SLEFT)
        registerInput(i, #INPUT_ControlStick_LEFT)
      EndIf 
      *port\controlStickBuffer[#DIRECTION_LEFT] = 0
ElseIf *port\currentControlStickState\x < -defaultControler\config\analogStickThreshold  And *port\previousState\axis[id]\x > -defaultControler\config\analogStickThreshold 
      *port\controlStickBuffer[#DIRECTION_LEFT] = 1
      *port\controlStickBuffer[#DIRECTION_RIGHT] = 0
    Else
      *port\controlStickBuffer[#DIRECTION_LEFT] = 0
      *port\controlStickBuffer[#DIRECTION_RIGHT] = 0
    EndIf
If *port\currentControlStickState\y > defaultControler\config\analogStickSmashThreshold 
If *port\previousState\axis[id]\y < defaultControler\config\analogStickThreshold  XOr *port\controlStickBuffer[#DIRECTION_RIGHT] = 1
        registerInput(i, #INPUT_ControlStick_SDOWN)
        registerInput(i, #INPUT_ControlStick_DOWN)
      EndIf 
      *port\controlStickBuffer[#DIRECTION_DOWN] = 0
ElseIf *port\currentControlStickState\y > defaultControler\config\analogStickThreshold  And *port\previousState\axis[id]\y < defaultControler\config\analogStickThreshold 
      *port\controlStickBuffer[#DIRECTION_UP] = 0
      *port\controlStickBuffer[#DIRECTION_DOWN] = 1
ElseIf *port\currentControlStickState\y < -defaultControler\config\analogStickSmashThreshold 
If *port\previousState\axis[id]\y > -defaultControler\config\analogStickThreshold  XOr *port\controlStickBuffer[#DIRECTION_LEFT] = 1
        registerInput(i, #INPUT_ControlStick_SUP)
        registerInput(i, #INPUT_ControlStick_UP)
      EndIf         
      *port\controlStickBuffer[#DIRECTION_UP] = 0
ElseIf *port\currentControlStickState\y < -defaultControler\config\analogStickThreshold  And *port\previousState\axis[id]\y > -defaultControler\config\analogStickThreshold 
      *port\controlStickBuffer[#DIRECTION_UP] = 1
      *port\controlStickBuffer[#DIRECTION_DOWN] = 0
    Else
      *port\controlStickBuffer[#DIRECTION_UP] = 0
      *port\controlStickBuffer[#DIRECTION_DOWN] = 0
    EndIf 
    *port\previousState\axis[id]\x = *port\currentControlStickState\x
    *port\previousState\axis[id]\y = *port\currentControlStickState\y
    ForEach defaultControler\defaultBinding\axises()
      id = defaultControler\defaultBinding\axises()\ID
      readAxis(@axisState, defaultControler\defaultBinding\axises()\ID, *port)
If axisState\x > defaultControler\config\analogStickThreshold  And *port\previousState\axis[id]\x < defaultControler\config\analogStickThreshold 
        registerInput(i, defaultControler\defaultBinding\axises()\input, id, 1)
ElseIf axisState\x < -defaultControler\config\analogStickThreshold  And *port\previousState\axis[id]\x > -defaultControler\config\analogStickThreshold 
        registerInput(i, defaultControler\defaultBinding\axises()\input, id, 1)
      EndIf 
If axisState\y > defaultControler\config\analogStickThreshold  And *port\previousState\axis[id]\y < defaultControler\config\analogStickThreshold 
        registerInput(i, defaultControler\defaultBinding\axises()\input, id, 1)
ElseIf  axisState\y < -defaultControler\config\analogStickThreshold  And *port\previousState\axis[id]\y > -defaultControler\config\analogStickThreshold 
        registerInput(i, defaultControler\defaultBinding\axises()\input, id, 1)
      EndIf 
      *port\previousState\axis[id]\x = axisState\x
      *port\previousState\axis[id]\y = axisState\y
    Next
    ForEach defaultControler\defaultBinding\triggers()
      axisState\z = readTrigger(defaultControler\defaultBinding\triggers()\ID, *port)
If axisState\z > defaultControler\config\analogTriggerThreshold  And *port\previousState\axis[defaultControler\defaultBinding\triggers()\ID]\z < defaultControler\config\analogTriggerThreshold 
        registerInput(i, defaultControler\defaultBinding\triggers()\input)
      EndIf 
      *port\previousState\axis[defaultControler\defaultBinding\triggers()\ID]\z = axisState\z
    Next 
  Next 
EndProcedure
Procedure inputManager_Attack(*port.Port, *info.inputData)
  Define direction.b
  Shared inputQ()
  If *info\stick
    state.AxisState
    readAxis(@state, *info\element, *port)
    direction = stickDirection(@state)
  Else 
    direction = controlStickDirection(*port)
  EndIf 
  If isFighterGrounded(*port\figher)
    ForEach inputQ()
      input.b = inputQ() & %11111
      If input = #INPUT_ControlStick_SDOWN Or input = #INPUT_ControlStick_SUP Or input = #INPUT_ControlStick_SLEFT Or input = #INPUT_ControlStick_SRIGHT
        DeleteElement(inputQ())
        Select input
          Case #INPUT_ControlStick_SUP
            Debug "Upsmash (" + *port\figher\name + ")"
          Case #INPUT_ControlStick_SDOWN
            Debug "Dsmash (" + *port\figher\name + ")"
          Case #INPUT_ControlStick_SLEFT, #INPUT_ControlStick_SRIGHT  
            Debug "FSmash (" + *port\figher\name + ")"
        EndSelect
        ProcedureReturn 1
      EndIf 
    Next 
    If *port\figher\state = #STATE_DASH
      Debug "Dash Attack (" + *port\figher\name + ")"
      ProcedureReturn 1
    EndIf
    Select direction
      Case #DIRECTION_NONE
        Debug "Jab (" + *port\figher\name + ")"
        ProcedureReturn 1
      Case #DIRECTION_RIGHT, #DIRECTION_LEFT
        Debug "Ftilt (" + *port\figher\name + ")"  
        ProcedureReturn 1
      Case #DIRECTION_UP
        Debug "Utilt (" + *port\figher\name + ")"
        ProcedureReturn 1
      Case #DIRECTION_DOWN
        Debug "Dtilt (" + *port\figher\name + ")"
        ProcedureReturn 1
    EndSelect 
  Else
    Select direction
      Case #DIRECTION_NONE
        Debug "Nair (" + *port\figher\name + ")"
        ProcedureReturn 1
      Case #DIRECTION_LEFT, #DIRECTION_RIGHT
        If direction - 1 = -*port\figher\facing
          Debug "Fair (" + *port\figher\name + ")"
        Else 
          Debug "Bair (" + *port\figher\name + ")"
        EndIf 
        ProcedureReturn 1
      Case #DIRECTION_UP
        Debug "Uair (" + *port\figher\name + ")"
      Case #DIRECTION_DOWN
        Debug "Dair (" + *port\figher\name + ")"
    EndSelect
  EndIf 
EndProcedure
*inputManagers(#INPUT_Attack) = @inputManager_Attack()
Procedure inputManager_smashStickRight(*port.Port, *info.inputData)
  Define state.b
  state = *port\figher\state
  If state = #STATE_WALK Or (state = #STATE_IDLE And *port\figher\grounded) Or state = #STATE_DASH_START
    *port\figher\facing = 1
    setState(*port\figher, #STATE_DASH_START)
  EndIf 
  If (state = #STATE_DASH Or state = #STATE_DASH_STOP) And *port\figher\facing = -1
    *port\figher\facing = 1
    setState(*port\figher, #STATE_DASH_TURN)
  EndIf 
  ProcedureReturn 1
EndProcedure
*inputManagers(#INPUT_ControlStick_RIGHT) = @inputManager_smashStickRight()
Procedure inputManager_smashStickLeft(*port.Port, *info.inputData)
  Define state.b
  state = *port\figher\state
  If state = #STATE_WALK Or (state = #STATE_IDLE And *port\figher\grounded) Or state = #STATE_DASH_START
    *port\figher\facing = -1
    setState(*port\figher, #STATE_DASH_START)
  EndIf 
  If (state = #STATE_DASH Or state = #STATE_DASH_STOP) And *port\figher\facing = 1
    *port\figher\facing = -1
    setState(*port\figher, #STATE_DASH_TURN)
  EndIf 
  ProcedureReturn 1
EndProcedure
*inputManagers(#INPUT_ControlStick_LEFT) = @inputManager_smashStickLeft()
Procedure inputManager_jump(*port.Port, *info.inputData)
  Shared defaultControler
  Define jumpType.b
  If *port\figher\grounded
    If *port\figher\state = #STATE_WALK Or *port\figher\state = #STATE_DASH
      jumpType = #JUMP_WALKING
    Else 
      jumpType = #JUMP_NORMAL
    EndIf 
    setState(*port\figher, #STATE_JUMPSQUAT, jumpType + (*info\element << 2))
  ElseIf *port\figher\jumps > 0
If Abs(*port\currentControlStickState\x) > defaultControler\config\analogStickThreshold 
      If  Sign(*port\currentControlStickState\x) = *port\figher\facing 
        jump(*port\figher, #JUMP_WALKING, #YJUMP_DOUBLE)
      Else
        jump(*port\figher, #JUMP_BACKWARDS, #YJUMP_DOUBLE)
      EndIf 
    Else
      jump(*port\figher, #JUMP_NORMAL, #YJUMP_DOUBLE)
    EndIf
    *port\figher\jumps - 1  
  EndIf 
EndProcedure
*inputManagers(#INPUT_Jump) = @inputManager_jump()
Procedure inputManager_smashStickDown(*port.Port, *info.inputData)
  If *port\figher\state = #STATE_IDLE And *port\figher\physics\v\y < 0
    Debug "fast fall"
    *port\figher\physics\v\y = -*port\figher\character\fastFallSpeed
  EndIf 
EndProcedure    
*inputManagers(#INPUT_ControlStick_DOWN) = @inputManager_smashStickDown()
Procedure inputManager_controlStickState(*port.Port) 
  Shared defaultControler
  If *port\figher\state = #STATE_IDLE
If Abs(*port\currentControlStickState\x) > defaultControler\config\analogStickThreshold 
      If *port\figher\grounded
        *port\figher\facing = Sign(*port\currentControlStickState\x)
        setState(*port\figher, #STATE_WALK)
      Else 
        If *port\figher\physics\v\x < *port\figher\character\maxAirSpeed And *port\figher\physics\v\x > -*port\figher\character\maxAirSpeed
          *port\figher\physics\v\x + *port\figher\character\airAcceleration * Sign(*port\currentControlStickState\x)
          If *port\figher\physics\v\x > *port\figher\character\maxAirSpeed 
            *port\figher\physics\v\x = *port\figher\character\maxAirSpeed 
          ElseIf *port\figher\physics\v\x < -*port\figher\character\maxAirSpeed 
            *port\figher\physics\v\x = -*port\figher\character\maxAirSpeed 
          EndIf
        EndIf 
      EndIf 
    EndIf 
  EndIf 
  If *port\figher\state = #STATE_WALK
If *port\currentControlStickState\x < defaultControler\config\analogStickThreshold  And *port\currentControlStickState\x > -defaultControler\config\analogStickThreshold 
      setState(*port\figher, #STATE_IDLE)
    EndIf
  EndIf
  If *port\figher\state = #STATE_DASH
If *port\currentControlStickState\x < defaultControler\config\analogStickThreshold  And *port\currentControlStickState\x > -defaultControler\config\analogStickThreshold 
      setState(*port\figher, #STATE_DASH_STOP)
    EndIf
  EndIf 
EndProcedure
Procedure updateInputs()
  Shared inputQ(), ports(), *inputManagers(), *port.Port
  Define input.b, durability.b, port.b, res.b, *currentElement, info.inputData
  readInputs()
  ForEach inputQ()
    input = inputQ() & %11111
    durability = (inputQ() & %111100000) >> 5
    port = (inputQ() & %111000000000) >> 9
    info\stick = (inputQ() & %1000000000000) >> 12
    info\element = (inputQ() & %11111 << 13) >> 13
    If *inputManagers(input)
      *currentElement = @inputQ()
      inputManager.inputManager = *inputManagers(input)
      res = inputManager(@ports(port), @info)
      ChangeCurrentElement(inputQ(), *currentElement)
      If res
        DeleteElement(inputQ())
        Continue 
      EndIf 
    EndIf 
    durability - 1
    If durability < 1
      DeleteElement(inputQ())
    Else
      inputQ() = makeInputValue(input, durability, port, element, stick)
    EndIf 
  Next 
  For i = 0 To 3
    If Not ports(i)\active
      Continue  
    EndIf 
    *port = ports(i)
    inputManager_controlStickState(*port)
  Next
EndProcedure
availableJosticks.b = InitJoystick()
Procedure stateCallback_Jumpsquat(*fighter.Fighter, stateinfo.l)
  Shared ports(), defaultControler
  Define jumpType.b, button.b, axis.AxisState
  jumpType = stateinfo & %11
  button = (stateinfo & %1111100) >> 2
If Abs(ports(*fighter\port)\currentControlStickState\x) > defaultControler\config\analogStickThreshold  And Sign(ports(*fighter\port)\currentControlStickState\x) <> *fighter\facing 
    jumpType = #JUMP_BACKWARDS
  EndIf 
  jump(*fighter, jumpType, 1 - readButton(button, ports(*fighter\port)))
EndProcedure
Procedure manageStates(*game.Game)
  Define *fighter.Fighter, max.b
  ForEach *game\fighters()
    *fighter = @*game\fighters()
    Select *fighter\state
      Case #STATE_JUMPSQUAT
        max = *fighter\character\jumpsquatDuration
        If max < 1
          max = *fighter\currentAnimation\frameCount
        EndIf 
        If *fighter\stateTimer >= max
          stateCallback_Jumpsquat(*fighter, *fighter\stateInfo)
        EndIf
      Case #STATE_DASH_STOP
        max = *fighter\character\dashStopDuration
        If max < 1
          max = *fighter\currentAnimation\frameCount
        EndIf 
        If *fighter\stateTimer >= max
          setState(*fighter, #STATE_IDLE)
        EndIf  
      Case #STATE_DASH_START
        max = *fighter\character\dashStartDuration
        If max < 1
          max = *fighter\currentAnimation\frameCount
        EndIf 
        If *fighter\stateTimer >= max
          setState(*fighter, #STATE_DASH)
        EndIf 
      Case #STATE_DASH_TURN
        max = *fighter\character\dashTurnDuration
        If max < 1
          max = *fighter\currentAnimation\frameCount
        EndIf 
        If *fighter\stateTimer >= max
          setState(*fighter, #STATE_DASH)
        EndIf 
    EndSelect
    *fighter\stateTimer + 1
  Next 
EndProcedure
#GRAVITY = 0.6
Procedure groundCollision(x.l, y.l) 
  If y < 0
    ProcedureReturn 1  
  EndIf 
  ProcedureReturn 0
EndProcedure
Procedure substractValue(*v1.Double, v2.d)
  Define sign.b
  sign = Sign(*v1\d)
  *v1\d = Abs(*v1\d)
  *v1\d - v2
  If *v1\d < 0.0
    *v1\d = 0.0
  Else 
    *v1\d * sign
  EndIf 
  ProcedureReturn v1
EndProcedure
Procedure applyPhysics(*game.Game)
  Shared ports()
  Define nx.l, ny.l, *fighter.Fighter
  ForEach *game\fighters()
    *fighter = @*game\fighters()
    If *fighter\facing = 0
      *fighter\facing = 1
    EndIf 
    If isFighterGrounded(*fighter)
      substractValue(@*fighter\physics\v\x, *fighter\character\traction)
    Else
      substractValue(@*fighter\physics\v\x, *fighter\character\airFriction)
    EndIf 
    Select *fighter\state
      Case #STATE_WALK  
        *fighter\physics\v\x = *fighter\character\walkSpeed * *fighter\facing
      Case #STATE_DASH
        *fighter\physics\v\x = *fighter\character\dashSpeed * *fighter\facing
      Case #STATE_DASH_START
        *fighter\physics\v\x = *fighter\character\initialDashSpeed * *fighter\facing
      Case #STATE_DASH_TURN
        *fighter\physics\v\x + *fighter\character\dashTurnAccel * *fighter\facing
    EndSelect
    If *fighter\physics\v\y > -*fighter\character\maxFallSpeed
      *fighter\physics\v\y - #GRAVITY
      If *fighter\physics\v\y < -*fighter\character\maxFallSpeed
        *fighter\physics\v\y = -*fighter\character\maxFallSpeed
      EndIf 
    EndIf 
    nx = *fighter\x + *fighter\physics\v\x
    ny = *fighter\y + *fighter\physics\v\y
    If groundCollision(nx, ny)
      *fighter\jumps = 1
      *fighter\grounded = 1
      *fighter\physics\v\y = 0
      ny = 0
    Else 
      *fighter\grounded = 0
    EndIf 
    *fighter\x = nx
    *fighter\y = ny
  Next 
EndProcedure
InitSprite()
UsePNGImageDecoder()
window.l = OpenWindow(-1, 0, 0, #SCREEN_W + 170, #SCREEN_H, "test", #PB_Window_SystemMenu | #PB_Window_ScreenCentered)
OpenWindowedScreen(WindowID(window), 0, 0, #SCREEN_W, #SCREEN_H, 0, 0, 0, #PB_Screen_NoSynchronization )
SetWindowColor(window, $aaaaaa)
loadGameData()
stateDefaultAnimation(#STATE_WALK) = "walk"
stateDefaultAnimation(#STATE_IDLE) = "idle"
stateDefaultAnimation(#STATE_DASH) = "dash"
stateDefaultAnimation(#STATE_JUMPSQUAT) = "jumpsquat"
stateDefaultAnimation(#STATE_DASH_STOP) = "dashStop"
stateDefaultAnimation(#STATE_DASH_START) = "dashStart"
stateDefaultAnimation(#STATE_DASH_TURN) = "dashTurn"
kuribrawl\variables\walkingJumpBoost = 3.0
kuribrawl\variables\backwardJumpBoost = 3.0
kuribrawl\variables\doubleJumpBackwardSpeed = 3.0
*c1.Champion = newCharacter("Acid Rainbows")
*testAnim.Animation = newAnimation(*c1, "idle", "Acid/idle", 0.05)
addLeftSpritesheet(*testAnim, "Acid/idleL")
addFrame(*testAnim, 0, 0, 34, 72, 17, 72)
addFrame(*testAnim, 34, 0, 34, 72, 17, 72)
addFrame(*testAnim, 68, 0, 34, 72, 17, 72)
addFrame(*testAnim, 102, 0, 34, 72, 17, 72)
resetAnimation(*testAnim)
*testAnim.Animation = newAnimation(*c1, "walk", "Acid/walk", 0.15)
addLeftSpritesheet(*testAnim, "Acid/walkL")
addFrame(*testAnim, 0, 0, 30, 70, 15, 70)
addFrame(*testAnim, 30, 0, 30, 70, 15, 70)
addFrame(*testAnim, 60, 0, 30, 70, 15, 70)
addFrame(*testAnim, 90, 0, 30, 70, 15, 70)
addFrame(*testAnim, 120, 0, 30, 70, 15, 70)
addFrame(*testAnim, 150, 0, 30, 70, 15, 70)
resetAnimation(*testAnim)
*testAnim = newAnimation(*c1, "dash", "Acid/dash", 0.18)
addLeftSpritesheet(*testAnim, "Acid/dashL")
addFrame(*testAnim, 0, 0, 62, 62, 31, 62)
addFrame(*testAnim, 62, 0, 62, 62, 31, 62)
addFrame(*testAnim, 124, 0, 62, 62, 31, 62)
addFrame(*testAnim, 186, 0, 62, 62, 31, 62)
resetAnimation(*testAnim)
*testAnim = newAnimation(*c1, "jumpsquat", "Acid/jumpsquat", -1)
addLeftSpritesheet(*testAnim, "Acid/jumpsquatL")
addFrame(*testAnim, 0, 0, 40, 56, 20, 56)
*testAnim = newAnimation(*c1, "dashStop", "Acid/dashStop")
addLeftSpritesheet(*testAnim, "Acid/dashStopL")
addFrame(*testAnim, 0, 0, 50, 68, 25, 68)
addFrame(*testAnim, 50, 0, 50, 68, 25, 68)
*testAnim = newAnimation(*c1, "dashStart", "Acid/dashStart", 0.25)
addLeftSpritesheet(*testAnim, "Acid/dashStartL")
addFrame(*testAnim, 0, 0, 44, 66, 22, 66)
addFrame(*testAnim, 44, 0, 44, 66, 22, 66)
*testAnim = newAnimation(*c1, "dashTurn", "Acid/dashTurn", 0.2)
addLeftSpritesheet(*testAnim, "Acid/dashTurnL")
addFrame(*testAnim, 0, 0, 44, 64, 22, 64)
addFrame(*testAnim, 44, 0, 44, 64, 22, 64)
addFrame(*testAnim, 88, 0, 44, 64, 22, 64)
*c1\dashSpeed = 8.0
*c1\initialDashSpeed = 10.0
*c1\dashTurnAccel = 1.5
*c1\walkSpeed = 4.5
*c1\maxAirSpeed = 5.0
*c1\airAcceleration = 0.3
*c1\airFriction = 0.05
*c1\traction = 0.7
*c1\jumpSpeed = 13.0
*c1\jumpsquatDuration = 5
*c1\shortHopSpeed = 7.5
*c1\doubleJumpSpeed = 12.0
*c1\fastFallSpeed = 16.0
*c1\maxFallSpeed = 11.0
*c1\dashStopDuration = 0
*c1\dashStartDuration = 0
*c1\dashTurnDuration = 0
*game.Game = initGame(window)
*f1.Fighter = newFighter(*game, getCharacter("Acid Rainbows"), 48, 500)
*f1\name = "Test One"
For i = 1 To availableJosticks
  Debug Str(i - 1) + JoystickName(i - 1)
Next
setPort(0, 0)
setPortFighter(0, *f1)
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
     EndIf 
     Delay(frameWait)
   EndIf 
  frame + 1
  endTime = ElapsedMilliseconds()
  lastFrameDuration = endTime - startTime
Until WindowEvent() = #PB_Event_CloseWindow
