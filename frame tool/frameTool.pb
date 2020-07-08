InitSprite()
IncludeFile "../src/filelib.pb"

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

Structure FrameModel
  display.Rect_
  origin.Vector
  actionnable.b
  duration.b
  List *collisionBoxes.CollisionBox()
EndStructure

Structure AnimationModel
  List frames.FrameModel()
  spriteSheet.l ;handle de l'image servant de spritesheet
  spriteSheetL.l;image pour les sprite retournés
  baseSpeed.d
EndStructure

Structure Champion
  Map animations.AnimationModel()
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

Global NewMap charList.Champion()

Procedure newCharacter(name.s)
  *r = AddMapElement(charList(), name)
  ProcedureReturn *r
EndProcedure

Procedure newAnimation(*character.Champion, name.s, spriteTag.s, speed.d = 1)
  Shared loadedSprites()
  *character\animations(name)\spriteSheet = loadedSprites(spriteTag)
  If Not loadedSprites(spriteTag)
    Debug "Can't find spritesheet with tag " + spriteTag
  EndIf
  *animation.AnimationModel = @*character\animations()
  
  *animation\baseSpeed = speed
  
  ProcedureReturn *animation
EndProcedure

Procedure addLeftSpritesheet(*animation.AnimationModel, tag.s)
  Shared loadedSprites()
  *animation\spriteSheetL = loadedSprites(tag)
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
EndProcedure


OpenWindow(0, 0, 0, 400, 210, "Frame Helper", #PB_Window_ScreenCentered)
OpenWindowedScreen(WindowID(0), 5, 5, 200, 200)


loadGameData("../src/res/data.twl")

IncludeFile "../src/animations.pbi"


; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 120
; FirstLine = 76
; Folding = -
; EnableXP