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
  damage.d
  hitstunModifier.l
  bkb.d ;base knockback
  skb.d ;scaling klockback
  angle.w
  hit.b
EndStructure  

Structure FrameModel
  display.Rect_
  origin.Vector
  actionnable.b
  duration.b
  List hurtboxes.Hurtbox()
  List hitboxes.Hitbox()
EndStructure

Prototype.i f_callback(*fighter, *data)

Structure AnimationModel
  List frames.FrameModel()
  spriteSheet.l ;handle de l'image servant de spritesheet
  spriteSheetL.l;image pour les sprite retournés
  baseSpeed.d
  endCallback.f_callback
EndStructure

Dim stateDefaultAnimation.s(#STATES)
Dim commandDefaultAnimation.s(#COMMANDS)

Structure moveInfo
  landLag.b
EndStructure

Structure Champion
  Map animations.AnimationModel()
  Array moves.moveInfo(#Commands)
  name.s
  displayName.s
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

Structure PlatformModel
  x.l
  y.l
  w.l
  animationName.s
EndStructure

Structure StageModel
  List platforms.platformModel()
  Map animations.AnimationModel()
EndStructure

Structure ArtifactModel
  Map animations.AnimationModel()
  durability.b
EndStructure

Structure GameVariables
  backwardJumpBoost.d
  walkingJumpBoost.d
  doubleJumpBackwardSpeed.d
EndStructure

Structure GameData
  Map characters.Champion()
  Map stages.StageModel()
  variables.GameVariables
EndStructure  
Global kuribrawl.GameData

InitSprite()

Procedure newCharacter(name.s)
  *r = AddMapElement(kuribrawl\characters(), name)
  kuribrawl\characters(name)\name = name
  kuribrawl\characters(name)\displayName = name
  ProcedureReturn *r
EndProcedure

Procedure getCharacter(name.s)
  ProcedureReturn FindMapElement(kuribrawl\characters(), name)
EndProcedure

Procedure getStage(name.s)
  ProcedureReturn FindMapElement(kuribrawl\stages(), name)
EndProcedure

Procedure getStageAnimation(*stage.StageModel, name.s)
  ProcedureReturn FindMapElement(*stage\animations(), name)
EndProcedure

Procedure getAnimation(*character.Champion, name.s)
  ProcedureReturn FindMapElement(*character\animations(), name)
EndProcedure

Procedure initAnimationModel(*animation.AnimationModel, spriteTag.s, speed.d = 1)
  Shared loadedSprites()
  If spriteTag
    *animation\spriteSheet = loadedSprites(spriteTag)
  EndIf 
  
  *animation\baseSpeed = speed
  
EndProcedure

Procedure newAnimation(*character.Champion, name.s, spriteTag.s, speed.d = 1)
  *animation.AnimationModel = AddMapElement(*character\animations(), name)
  initAnimationModel(*animation, spriteTag, speed)
  ProcedureReturn *animation
EndProcedure

Procedure addLeftSpritesheet(*animation.AnimationModel, tag.s)
  Shared loadedSprites()
  *animation\spriteSheetL = loadedSprites(tag)
EndProcedure

Procedure setAnimationEndCallback(*animation.AnimationModel, f.f_callback)
  *animation\endCallback = f
EndProcedure

Procedure addFrame(*animation.AnimationModel, x.l, y.l, w.l, h.l, xo.l, yo.l, duration.b = 0)
  *f.FrameModel = AddElement(*animation\frames())
  *f\display\x = x
  *f\display\y = y
  *f\display\w = w
  *f\display\h = h
  *f\origin\x = xo
  *f\origin\y = yo
  *f\duration = duration
  ProcedureReturn *f
EndProcedure

Procedure addHitbox(*frame.FrameModel, x.l, y.l, w.l, h.l, shape = #CBOX_SHAPE_RECT)
  *r.Hitbox = AddElement(*frame\hitboxes())
  Select shape
    Case #CBOX_SHAPE_RECT
      *r\x = x
      *r\y = y
      *r\x2 = w
      *r\y2 = h
  EndSelect
  *r\shape = shape
  ProcedureReturn *r
EndProcedure

Procedure addHurtbox(*frame.FrameModel, x.l, y.l, w.l, h.l, shape = #CBOX_SHAPE_RECT)
  *r.Hurtbox = AddElement(*frame\hurtboxes())
  Select shape
    Case #CBOX_SHAPE_RECT
      *r\x = x
      *r\y = y
      *r\x2 = w
      *r\y2 = h
  EndSelect
  *r\shape = shape
  ProcedureReturn *r
EndProcedure

Procedure newStage(name.s)
  ProcedureReturn AddMapElement(kuribrawl\stages(), name)  
EndProcedure

Procedure addPlatform(*stage.StageModel, x.l, y.l, w.l, animationName.s = "")
  Define *r.PlatformModel
  *r = AddElement(*stage\platforms())
  *r\x = x
  *r\y = y
  *r\w = w
  *r\animationName = animationName
  ProcedureReturn *r
EndProcedure

Procedure newStageAnimation(*stage.StageModel, name.s, spriteTag.s, speed.d = 1)
  *animation = AddMapElement(*stage\animations(), name)
  initAnimationModel(*animation, spriteTag, speed)
  ProcedureReturn *animation
EndProcedure

Declare defaultJumpAnimCallback(*fighter, *data)
Declare defaultAttackAnimCallback(*fighter, *data)
Procedure initDefaultAnimationsConfig(*char.Champion)
  Shared commandDefaultAnimation()
  If *char\animations("jump")
    setAnimationEndCallback(@*char\animations("jump"), @defaultJumpAnimCallback())
  EndIf 
  For i = 0 To #COMMANDS - 1
    If *char\animations(commandDefaultAnimation(i))
      setAnimationEndCallback(*char\animations(commandDefaultAnimation(i)), @defaultAttackAnimCallback())
    EndIf
  Next 
EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 112
; FirstLine = 73
; Folding = ---
; EnableXP