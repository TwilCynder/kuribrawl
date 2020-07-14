frame.l = 0


Structure Frame
  *model.FrameModel
  timeLeft.d
EndStructure  

Structure Animation
  *model.AnimationModel
  List frames.Frame()
  speed.d
  frameMultiplier.b
  carry.f ;for animations with speed
  currentCarry.f
  endCallback.f_callback
EndStructure  

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

Procedure resetAnimation(*animation.Animation)
  
  ResetList(*animation\frames())
  NextElement(*animation\frames())
  If *animation\frameMultiplier > 1
    *animation\currentCarry = 0
    *animation\frames()\timeLeft = *animation\frameMultiplier
  EndIf 
EndProcedure

Procedure framesCount(*anim.Animation)
  ProcedureReturn ListSize(*anim\frames())
EndProcedure

;nécessite un setAnimation préalable
Procedure animLength(*anim.Animation)
  Define multiplier.b
  If *anim\speed > 1
    ProcedureReturn *anim\speed
  Else
    ProcedureReturn ListSize(*anim\frames()) * Abs(Int(1 / *anim\speed))
  EndIf 
EndProcedure

Procedure setAnimationSpeed(*anim.Animation, speed.d = 0)
  Define ratio.d
  If speed <> 0
    *anim\speed = speed
  Else
    *anim\speed = *anim\model\baseSpeed
  EndIf 
  
  If *anim\speed > 1
    ratio = *anim\speed / framesCount(*anim)
  Else
    ratio = 1 / *anim\speed
  EndIf 
  *anim\frameMultiplier = ratio
  resetAnimation(*anim)
    
  *anim\carry = ratio - Int(ratio)
EndProcedure

Procedure setAnimation(*fighter.Fighter, name.s, speed.d = 0)
  Define *anim.Animation
  If *fighter\currentAnimationName = name
    ProcedureReturn 0
  EndIf
  
  *anim = FindMapElement(*fighter\animations(), name)
 
  If Not *anim
    Debug "Can't set animation to " + name
    ProcedureReturn 0
  EndIf 
  *fighter\currentAnimation = *anim
  *fighter\currentAnimationName = name
  ;Debug *anim

  setAnimationSpeed(*anim, speed)
  resetAnimation(*anim)
EndProcedure

Procedure newFighter(*game.Game, *character.Champion, x.l, y.l, port = -1)
  
  Define *anim.Animation, *model.AnimationModel
  *r.Fighter = AddElement(*game\fighters())
  *r\x = x
  *r\y = y
  *r\physics\v\x = 0
  *r\physics\v\y = 0
  *r\physics\a\x = 0
  *r\physics\a\y = 0
  *r\character = *character
  
  ForEach *character\animations()
    *model = @*character\animations()
    *anim = AddMapElement(*r\animations(), MapKey(*character\animations()))
    *anim\endCallback = *model\endCallback
    *anim\model = *model
    
    ForEach *model\frames()
      AddElement(*anim\frames())
      *anim\frames()\model = @*model\frames()
    Next 
    setAnimation(*r, MapKey(*character\animations()))
  Next 
  *r\state = #STATE_IDLE
  
  *r\port = port
  ProcedureReturn *r
EndProcedure

Procedure drawAnimationFrame(*frame.FrameModel, spriteSheet.l, x.l, y.l)
  With *frame
    ClipSprite(spriteSheet, \display\x, \display\y, \display\w, \display\h)
    DisplayTransparentSprite(spriteSheet, x - \origin\x, y - \origin\y)
  EndWith
EndProcedure

Procedure renderFighter(*fighter.Fighter)
  Define spriteSheet.l
  If *fighter\facing = -1 And *fighter\currentAnimation\model\spriteSheetL
    spriteSheet = *fighter\currentAnimation\model\spriteSheetL
  Else
    spriteSheet = *fighter\currentAnimation\model\spriteSheet
  EndIf 
  drawAnimationFrame(*fighter\currentAnimation\frames()\model, spriteSheet, *fighter\x, #SCREEN_H - *fighter\y)
EndProcedure

Procedure renderFrame(*game.Game)
  ClearScreen(#White)
  ForEach *game\fighters()
    renderFighter(@*game\fighters())
  Next
  FlipBuffers()
EndProcedure

Procedure nextFrame(*animation.Animation, *fighter.Fighter)
  If NextElement(*animation\frames()) = 0
    If *animation\endCallback
      ProcedureReturn *animation\endCallback(*fighter, 0)
    EndIf 
    resetAnimation(*animation)
  ElseIf *animation\frames()\model\duration
    *animation\frames()\timeLeft = *animation\frames()\model\duration
  ElseIf Not *animation\speed = 1
    *animation\frames()\timeLeft = *animation\frameMultiplier
    If *animation\currentCarry >= 1
      *animation\currentCarry - 1
      *animation\frames()\timeLeft - 1
    EndIf 
  EndIf
EndProcedure

Procedure advanceAnimations(*game.Game)
  Define *fighter.Fighter
  ForEach *game\fighters()
    *fighter = *game\fighters()
    If *fighter\currentAnimationName = "land"
    EndIf 
    If *fighter\currentAnimation\frames()\timeLeft >= 1
      *fighter\currentAnimation\frames()\timeLeft - 1
    ElseIf Not *fighter\currentAnimation\speed = -1
      *fighter\currentAnimation\currentCarry + *fighter\currentAnimation\carry
      nextFrame(*fighter\currentAnimation, *fighter)
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

Procedure attack(*fighter, attack.b)
  setState(*fighter, #STATE_ATTACK, attack)
EndProcedure

;système d'acceptable animations

Procedure updateAnimations(*game.Game)
  Shared stateDefaultAnimation(), commandDefaultAnimation()
  Define animation.s, *fighter.Fighter
  ForEach *game\fighters()
    *fighter = @*game\fighters()
    If *fighter\stateUpdated
      ;case by case
      Select *fighter\state
        Case #STATE_IDLE
          If *fighter\grounded = 0
            If *fighter\stateInfo & 1
              setAnimation(*fighter, "jump")
            Else
              setAnimation(*fighter, "airIdle")
            EndIf 
          ElseIf *fighter\grounded = 1
            setAnimation(*fighter, "idle")
          EndIf 
        Case #STATE_ATTACK
          setAnimation(*fighter, commandDefaultAnimation(*fighter\stateInfo))
        Default 
          animation = stateDefaultAnimation(*fighter\state)
          If animation And Not animation = *fighter\currentAnimationName
            setAnimation(*fighter, animation)
          EndIf 
      EndSelect
      
      If *fighter\state = #STATE_LANDING_LAG
        setAnimationSpeed(*fighter\currentAnimation, *fighter\stateInfo)
      EndIf
    EndIf
    *fighter\stateUpdated = 0
  Next 
EndProcedure

Procedure defaultJumpAnimCallback(*fighter.Fighter, *data)
  setAnimation(*fighter, "airIdle")
EndProcedure

Procedure defaultAttackAnimCallback(*fighter.Fighter, *data)
  setState(*fighter, #STATE_IDLE)
EndProcedure

Procedure initFighters(*game.Game)
  ForEach *game\fighters()
    *game\fighters()\grounded = 0
    setState(*game\fighters(), #STATE_IDLE, 0)
  Next 
  updateAnimations(*game)
EndProcedure


; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 157
; FirstLine = 136
; Folding = ----
; EnableXP
; SubSystem = OpenGL
; EnableUnicode