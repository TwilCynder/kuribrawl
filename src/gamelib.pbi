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

Enumeration #PB_Event_FirstCustomValue

EndEnumeration

Structure Physics
  v.Vector
  a.Vector
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
EndStructure  
  
Structure Fighter
  x.l
  y.l
  physics.Physics
  Map animations.Animation()
  *currentAnimation.Animation ;on pourrait s'en passer mais avoir un ptr vers l'objet évite de faire un accès map à chaque fois
  currentAnimationName.s
  port.b
  name.s
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

Procedure newFighter(*game.Game, x, y, port = -1)
  *r.Fighter = AddElement(*game\fighters())
  *r\x = x
  *r\y = y
  *r\physics\v\x = 0
  *r\physics\v\y = 0
  *r\physics\a\x = 0
  *r\physics\a\y = 0
  
  *r\port = port
  ProcedureReturn *r
EndProcedure

Procedure newAnimation(*fighter.Fighter, name.s, spritePath.s, current = 0, speed.d = 1)
  *fighter\animations(name)\spriteSheet = LoadSprite(-1, spritePath)
  *animation.Animation = @*fighter\animations()
  
  If current
    *fighter\currentAnimation = *animation
    *fighter\currentAnimationName = name
  EndIf
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
EndProcedure

Procedure renderFrame(*game.Game)
  Define y.l
  ClearScreen(0)
  ForEach *game\fighters()
    With *game\fighters()\currentAnimation\frames()
      y = #SCREEN_H - *game\fighters()\y - \origin\y
      ClipSprite(*game\fighters()\currentAnimation\spriteSheet, \display\x, \display\y, \display\w, \display\h)
      DisplaySprite(*game\fighters()\currentAnimation\spriteSheet, *game\fighters()\x - \origin\x, y)
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
  ProcedureReturn 1
EndProcedure

Procedure figherDirection(*fighter.Fighter)
  ProcedureReturn 0
EndProcedure


; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 161
; FirstLine = 138
; Folding = --
; EnableXP
; SubSystem = OpenGL
; EnableUnicode