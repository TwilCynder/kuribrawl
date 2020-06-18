frame.l = 0

XIncludeFile "gamelibConstants.pb"

Structure Physics
  v.VectorDouble
  a.VectorDouble
EndStructure

Structure CollisionBox
  x.l
  y.l
  shape.b
  s.l  ;"size" = side length for a square, radius for a circle
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
  bkb.d ;base knockback
  skb.d ;scaling klockback
  angle.d
  hit.b
EndStructure  
 
Structure Frame
  display.Rect_
  origin.Vector
  actionnable.b
  actions.b[#ACTIONS_TOTAL] ;1 = bloqué
  List *collisionBoxes.CollisionBox()
  *callback.f_callback
  duration.b
EndStructure  
  
Structure Animation
  spriteSheet.l ;handle de l'image servant de spritesheet
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
  *currentAnimation.Animation ;on pourrait s'en passer mais avoir un ptr vers l'objet évite de faire un accès map à chaque fois
  currentAnimationName.s
  port.b
  name.s
  grounded.b
  facing.b ;1 = right | -1 = left
  state.b
  stateInfo.b ;attack, direction
  stateTimer.u
  jumps.b
EndStructure

Structure Game
  List fighters.Fighter()
  window.l
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
  previousState.ControllerState
  currentControlStickState.AxisState
  controlStickBuffer.b[4]
  active.b
  *figher.Fighter
EndStructure
Dim ports.Port(4)

Declare readButton(button.b, *port.Port)
Declare readAxis(*state.AxisState, axis.b, *port.Port)

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

Procedure newAnimation(*character.Champion, name.s, spritePath.s, speed.d = 1)
  *character\animations(name)\spriteSheet = LoadSprite(-1, spritePath, #PB_Sprite_AlphaBlending )
  *animation.Animation = @*character\animations()
  
  *animation\frameMultiplier = Int(1 / speed)
  ProcedureReturn *animation
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
    CopyList(*character\animations()\frames(), *anim\frames())
    setAnimation(*r, MapKey(*character\animations()))
  Next 
  *r\state = #STATE_IDLE
  
  *r\port = port
  
  ProcedureReturn *r
EndProcedure

Procedure renderFrame(*game.Game)
  Define y.l
  ClearScreen(#White)
  ForEach *game\fighters()
    With *game\fighters()\currentAnimation\frames()
      y = #SCREEN_H - *game\fighters()\y - \origin\y
      ClipSprite(*game\fighters()\currentAnimation\spriteSheet, \display\x, \display\y, \display\w, \display\h)
      DisplayTransparentSprite(*game\fighters()\currentAnimation\spriteSheet, *game\fighters()\x - \origin\x, y)
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

CompilerIf #DEBUG
  Declare logState(state.b, facing.b = 0, previousTime.b = 0)
CompilerEndIf
Procedure setState(*fighter.Fighter, state.b, info.l = 0)
  *fighter\state = state
  *fighter\stateInfo = info
  CompilerIf #DEBUG
    logState(state, *fighter\facing, *fighter\stateTimer)
  CompilerEndIf
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

Procedure stateCallback_Jumpsquat(*fighter.Fighter, stateinfo.l)
  Shared ports()
  Define jumpType.b, button.b, axis.AxisState
  
  jumpType = stateinfo & %11
  button = (stateinfo & %1111100) >> 2
  
  If Abs(ports(*fighter\port)\currentControlStickState\x) > stickTreshold And Sign(ports(*fighter\port)\currentControlStickState\x) <> *fighter\facing 
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


; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 302
; FirstLine = 297
; Folding = ----
; EnableXP
; SubSystem = OpenGL
; EnableUnicode