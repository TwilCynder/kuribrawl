XIncludeFile "gamelibConstants.pb"

;- Structures

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
  priority.b  
EndStructure  

Structure Frame
  display.Rect_ ;part of the source image that this frames displays
  origin.Vector ;position of the origin of this frame (relative to the upleft corner of display). The origin is the point that will be at the actual position of the object that displays the animation
  speed.VectorDouble ;speed applied to the object while this frame is active (if the object is a Fighter)
  speedMode.b ;bit 0 : {0 : disabled | 1 : enabled} ; bit 1 : {0 : speed is added | 1 : speed is set} ; bit 2 : {0 : when this frame comes out | 1 : during the whole frame}
  duration.b ;duration of this frame (overrides the animation speed, i.e. if duration != 0, the frame will always be displayed this many frames)
  List hurtboxes.Hurtbox()
  List hitboxes.Hitbox()
EndStructure

Prototype.i f_callback(*fighter, *data)

Structure AnimationModel
  noCollisions.b ;indicates that this animation doesn't interact with fighters (no hit/hurtboxes)
  frameNb.b
  Array frames.Frame(0)
  spriteSheet.l ;handle de l'image servant de spritesheet
  spriteSheetL.l;image pour les sprite retournés
  baseSpeed.d
  endCallback.f_callback
  frameCallback.f_callback
EndStructure

Dim stateDefaultAnimation.s(#STATES)
Dim commandDefaultAnimation.s(#COMMANDS)

Structure MultiMoveInfo
  Array partEndFrames.b(0)
  Array partStartFrames.b(0)
EndStructure

Structure MoveInfo
  landLag.b
  *multiMove.MultiMoveInfo
EndStructure

Structure ShieldInfo Extends Vector
  size.b
EndStructure

Structure ChampionAssets
  HUDIcon.l
EndStructure

;si ajout de variable, penser à update : loadLib, DFM, doc
Structure Champion
  Map animations.AnimationModel()
  Array moves.moveInfo(#Commands)
  assets.ChampionAssets
  name.s
  displayName.s
  walkSpeed.d
  dashSpeed.d
  initialDashSpeed.d
  dashTurnAccel.d
  maxAirSpeed.d
  airAcceleration.d
  airFriction.d
  traction.d
  jumpSpeed.d
  shorthopSpeed.d
  doubleJumpSpeed.d
  maxFallSpeed.d
  fastFallSpeed.d
  jumpsquatDuration.b
  dashStopDuration.b
  dashStartDuration.b
  dashTurnDuration.b
  landingDuration.b
  shieldStartup.b
  shieldEndlag.b
  shieldInfo.ShieldInfo
  weight.d
EndStructure

Structure PlatformModel
  x.l
  y.l
  w.l
  animationName.s
EndStructure

Structure StageModel
  name.s
  displayName.s
  List platforms.platformModel()
  Map animations.AnimationModel()
  w.l
  h.l
  cameraZone.RECT
  *backgroundAnim.AnimationModel  ;pointer to one the the animModels of tha animations map
EndStructure

Structure GameVariables
  backwardJumpBoost.d
  walkingJumpBoost.d
  doubleJumpBackwardSpeed.d
  shieldStartup.b
  shieldEndlag.b
  minimumShieldDuration.b
  shieldDecay.d
  shieldRegen.d
  cameraMaxSpeed.d
  
  knockbackMult.d
  knockbackBase.d
EndStructure

Structure Font
  fontImage.l
  fontDimensions.Vector
EndStructure

Structure HUDInfo
  damageFont.Font
EndStructure

Structure GameData
  Map characters.Champion()
  Map stages.StageModel()
  variables.GameVariables
  HUD.HUDInfo
EndStructure  
Global kuribrawl.GameData

InitSprite()

Procedure initHUD()
  Shared loadedSprites()
  kuribrawl\HUD\damageFont\fontImage = loadedSprites("damages_font")
  kuribrawl\HUD\damageFont\fontDimensions\x = 15
  kuribrawl\HUD\damageFont\fontDimensions\y = 30
EndProcedure

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

Procedure initAnimationModel(*animation.AnimationModel, spriteTag.s, speed.d = 1) ;associates a sprite handle with an Animation model, and maybe sets its speed
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

Procedure setAnimationFrameNumber(*animation.AnimationModel, n.b) ;by default AnimationModels are created with one frame. This changes the size of their internal Frames array
  ReDim *animation\frames(n - 1)
  *animation\frameNb = n
EndProcedure

Procedure framesCount(*anim.AnimationModel)
  ProcedureReturn ArraySize(*anim\frames()) + 1
EndProcedure

Procedure newChampionAnimation(*character.Champion, name.s, spriteTag.s, speed.d = 1) ;creates an animation in a champion's animation map, and initializes it.
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

Procedure AnimBaseLength(*animation.AnimationModel)
  If *animation\baseSpeed > 1 Or *animation\baseSpeed = -1
    ProcedureReturn *animation\baseSpeed
  Else
    ProcedureReturn ArraySize(*animation\frames()) * Int(1 / *animation\baseSpeed)
  EndIf 
EndProcedure

Procedure setFrameProperties(*f.Frame, x.l, y.l, w.l, h.l, xo.l, yo.l, duration.b = 0) ;sets the basic properties of a Frame
  *f\display\x = x
  *f\display\y = y
  *f\display\w = w
  *f\display\h = h
  *f\origin\x = xo
  *f\origin\y = yo
  *f\duration = duration
  ProcedureReturn *f
EndProcedure

Procedure addHitbox(*frame.Frame, x.l, y.l, w.l, h.l, shape = #CBOX_SHAPE_RECT)
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

Procedure addHurtbox(*frame.Frame, x.l, y.l, w.l, h.l, shape = #CBOX_SHAPE_RECT)
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

Procedure makeMultiMove(*attack.MoveInfo, nbMoves)
  If nbMoves < 2
    nbMoves = 2
  EndIf 
  *attack\multiMove = AllocateStructure(MultiMoveInfo)
  ReDim *attack\multiMove\partEndFrames(nbMoves - 1)
  ReDim *attack\multiMove\partStartFrames(nbMoves - 1)
EndProcedure

Procedure newStage(name.s, w.l = #SCREEN_W, h.l = #SCREEN_H)
  Define *stage.StageModel
  *stage = AddMapElement(kuribrawl\stages(), name)
  *stage\w = w
  *stage\h = h
  *stage\cameraZone\left = 0
  *stage\cameraZone\top = 0
  *stage\cameraZone\bottom = #SCREEN_H
  *stage\cameraZone\right = #SCREEN_W
  *stage\name = name
  *stage\displayName = name
  ProcedureReturn *stage
EndProcedure

Procedure setStageCameraZone(*stage.StageModel, x.l, y.l, w.l, h.l)
  If x = -1
    *stage\cameraZone\left = (*stage\w - w) / 2
  Else
    *stage\cameraZone\left = x
  EndIf 
  *stage\cameraZone\right = *stage\cameraZone\left + w
  If y = -1
    *stage\cameraZone\bottom = (*stage\h - h) / 2
  Else
    *stage\cameraZone\bottom = y
  EndIf 
  *stage\cameraZone\top = *stage\cameraZone\bottom + h
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

Procedure addCenteredPlatform(*stage.StageModel, y.l, w.l, animationName.s = "")
  addPlatform(*stage, (*stage\w / 2) - (w / 2), y, w, animationName)
EndProcedure 

Procedure newStageAnimation(*stage.StageModel, name.s, spriteTag.s, speed.d = 1)
  *animation = AddMapElement(*stage\animations(), name)
  initAnimationModel(*animation, spriteTag, speed)
  ProcedureReturn *animation
EndProcedure

Declare defaultJumpAnimCallback(*fighter, *data)
Declare defaultAttackAnimCallback(*fighter, *data)
Procedure initDefaultAnimationsConfig(*char.Champion)
  Define *anim.AnimationModel
  Shared commandDefaultAnimation()
  If *char\animations("jump")
    setAnimationEndCallback(@*char\animations("jump"), @defaultJumpAnimCallback())
    setAnimationEndCallback(@*char\animations("doublejump"), @defaultJumpAnimCallback())
  EndIf 
  
  For i = 0 To #COMMANDS - 1
    If getAnimation(*char, commandDefaultAnimation(i))
      setAnimationEndCallback(*char\animations(commandDefaultAnimation(i)), @defaultAttackAnimCallback())
    EndIf
  Next 
  
EndProcedure

Procedure.s getChampionAssetTag(championName.s, assetName.s)
  ProcedureReturn "@" + championName + "/" + assetName
EndProcedure

Procedure initChampion(*char.Champion)
  Shared loadedSprites()
 
  initDefaultAnimationsConfig(*char)
  *char\assets\HUDIcon = loadedSprites(getChampionAssetTag(*char\name, "hud_icon"))
EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 342
; FirstLine = 307
; Folding = -----
; EnableXP