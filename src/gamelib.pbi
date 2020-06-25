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
  List *collisionBoxes.CollisionBox()
  duration.b
EndStructure  

Prototype.i f_callback(*fighter, *data)

Structure Animation
  spriteSheet.l ;handle de l'image servant de spritesheet
  spriteSheetL.l ;image pour les sprite retournés
  List frames.Frame()
  frameMultiplier.b
  frameCount.b
  endCallback.f_callback
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
  landingDuration.d
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
  stateUpdated.b ;wheter state has been changed since last animation change
  jumps.b
EndStructure

Structure Game
  List fighters.Fighter()
  window.l
EndStructure

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
  If Not loadedSprites(spriteTag)
    Debug "Can't find spritesheet with tag " + spriteTag
  EndIf
  *animation.Animation = @*character\animations()
  
  *animation\frameMultiplier = Int(1 / speed)
  ProcedureReturn *animation
EndProcedure

Procedure addLeftSpritesheet(*animation.Animation, tag.s)
  Shared loadedSprites()
  *animation\spriteSheetL = loadedSprites(tag)
EndProcedure

Procedure setAnimationEndCallback(*animation.Animation, f.f_callback)
  *animation\endCallback = f
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
    *anim\endCallback = *character\animations()\endCallback
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
      y = #SCREEN_H - *fighter\y - \origin\y
      ClipSprite(spriteSheet, \display\x, \display\y, \display\w, \display\h)
      DisplayTransparentSprite(spriteSheet, *fighter\x - \origin\x, y)
    EndWith
  Next
  FlipBuffers()
EndProcedure

Procedure NextFrame(*game.Game)
  Define *fighter.Fighter
  ForEach *game\fighters()
    *fighter = *game\fighters()
    If *fighter\currentAnimation\frames()\duration > 1
      *fighter\currentAnimation\frames()\duration - 1
    ElseIf NextElement(*fighter\currentAnimation\frames()) = 0
      If *fighter\currentAnimation\endCallback
        ProcedureReturn *fighter\currentAnimation\endCallback(*fighter, 0)
      EndIf 
      resetAnimation(*fighter\currentAnimation)
    ElseIf *fighter\currentAnimation\frameMultiplier > 1
      *fighter\currentAnimation\frames()\duration = *fighter\currentAnimation\frameMultiplier
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
  *fighter\stateUpdated = 1
  
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
  setState(*fighter, #STATE_IDLE, 1)
EndProcedure

;système d'acceptable animations

Procedure updateAnimations(*game.Game)
  Shared stateDefaultAnimation()
  Define animation.s, *fighter.Fighter
  ForEach *game\fighters()
    *fighter = @*game\fighters()
    If *fighter\stateUpdated
      animation = stateDefaultAnimation(*fighter\state)
      If animation And Not animation = *fighter\currentAnimationName
        setAnimation(*fighter, animation)
      EndIf 
      ;case by case
      If *fighter\state = #STATE_IDLE
        If *fighter\grounded = 0
          If *fighter\stateInfo & 1
            setAnimation(*fighter, "jump")
          Else
            setAnimation(*fighter, "airIdle")
          EndIf 
        ElseIf *fighter\grounded = 1
          setAnimation(*fighter, "idle")
        EndIf 
      EndIf
    EndIf
    *fighter\stateUpdated = 0
  Next 
EndProcedure

Procedure defaultJumpAnimCallback(*fighter.Fighter, *data)
  setAnimation(*fighter, "airIdle")
EndProcedure

Procedure initDefaultAnimationsConfig(*char.Champion)
  If *char\animations("jump")
    setAnimationEndCallback(@*char\animations("jump"), @defaultJumpAnimCallback())
  EndIf 
EndProcedure


; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 35
; FirstLine = 21
; Folding = ----
; EnableXP
; SubSystem = OpenGL
; EnableUnicode