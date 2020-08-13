frame.l = 0
Global bgc.l = #White

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
  facing.b
  endCallback.f_callback
EndStructure  

Structure fighterHitIdPair
  *fighter
  hitID.b
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
  stateInfo.l ;attack, direction
  stateTimer.u
  updateAnim.b ;wheter state has been changed since last animation change
  jumps.b
  paused.b
  List fightersHit.fighterHitIdPair()
EndStructure

Structure Platform
  *animation.Animation
  *model.PlatformModel
EndStructure

Structure Stage
  List platforms.Platform()
EndStructure

Structure Game
  List fighters.Fighter()
  *currentStage.Stage
  window.l
EndStructure

Procedure initAnimation(*anim.Animation, *model.AnimationModel)
  *anim\endCallback = *model\endCallback
  *anim\model = *model
    
  ForEach *model\frames()
    AddElement(*anim\frames())
    *anim\frames()\model = @*model\frames()
  Next 
EndProcedure

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

Procedure setStage(*game.Game, *model.StageModel)
  Define *model.StageModel
  If Not *model
    ProcedureReturn 0
  EndIf 
  *game\currentStage = AllocateStructure(Stage)
  ;set the stage model if it's useful one day
  ForEach *model\platforms()
    AddElement(*game\currentStage\platforms())
    *game\currentStage\platforms()\model = *model\platforms()
    If *model\platforms()\animationName
      *game\currentStage\platforms()\animation = AllocateStructure(Animation)
      Debug MapSize(*model\animations())
      initAnimation(*game\currentStage\platforms()\animation, getStageAnimation(*model, *model\platforms()\animationName))
      setAnimationSpeed(*game\currentStage\platforms()\animation)
    EndIf 
  Next 
EndProcedure 

Procedure setAnimation(*fighter.Fighter, name.s, speed.d = 0, facing = 0)
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
  
  *fighter\currentAnimation\facing = facing
  
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
    initAnimation(*anim, *model)
    setAnimation(*r, MapKey(*character\animations()))
  Next 
  *r\state = #STATE_IDLE
  
  *r\port = port
  ProcedureReturn *r
EndProcedure

Procedure getRealCboxX(*cbox.CollisionBox, facing)
  If facing = 1
    ProcedureReturn *cbox\x
  Else
    ProcedureReturn - *cbox\x - *cbox\x2
  EndIf 
EndProcedure

Procedure drawAnimationFrame(*frame.FrameModel, spriteSheet.l, x.l, y.l, facing)
  Define dx.l
  With *frame
    If facing = 1
      dx = x - \origin\x
    Else
      dx = x + \origin\x - \display\w
    EndIf
    ClipSprite(spriteSheet, \display\x, \display\y, \display\w, \display\h)
    DisplayTransparentSprite(spriteSheet, dx, y - \origin\y)
    CompilerIf #DEBUG
      StartDrawing(ScreenOutput())
      DrawingMode(#PB_2DDrawing_Outlined)
      Line(0, y, #SCREEN_W, 1, #Blue)
      ForEach *frame\hitboxes()
        Box(x + getRealCboxX(*frame\hitboxes(), facing), y - *frame\hitboxes()\y, *frame\hitboxes()\x2, *frame\hitboxes()\y2, #Red)
      Next
      ForEach *frame\hurtboxes()
        Box(x + getRealCboxX(*frame\hurtboxes(), facing), y - *frame\hurtboxes()\y, *frame\hurtboxes()\x2, *frame\hurtboxes()\y2, #Green)
      Next
      StopDrawing()
    CompilerEndIf
  EndWith
EndProcedure

Procedure renderFighter(*fighter.Fighter)
  Define spriteSheet.l, facing.b
  If *fighter\currentAnimation\facing = 0
    facing = *fighter\facing
  Else
    facing = *fighter\currentAnimation\facing
  EndIf 
  If facing = -1 And *fighter\currentAnimation\model\spriteSheetL
    spriteSheet = *fighter\currentAnimation\model\spriteSheetL
  Else
    spriteSheet = *fighter\currentAnimation\model\spriteSheet
  EndIf 
  drawAnimationFrame(*fighter\currentAnimation\frames()\model, spriteSheet, *fighter\x, #SCREEN_H - *fighter\y, facing)
EndProcedure

Procedure renderPlatform(*platform.Platform)
  drawAnimationFrame(*platform\animation\frames()\model, *platform\animation\model\spriteSheet, *platform\model\x, #SCREEN_H - *platform\model\y, 1)
EndProcedure

Procedure renderFrame(*game.Game)
  ClearScreen(bgc)
  ForEach *game\fighters()
    renderFighter(@*game\fighters())
  Next
  ForEach *game\currentStage\platforms()
    renderPlatform(@*game\currentStage\platforms())
  Next
  FlipBuffers()
EndProcedure

Procedure nextFrame(*animation.Animation, *fighter.Fighter)
  If *fighter\paused > 0
    ProcedureReturn 0
  EndIf 
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
  *fighter\updateAnim = 1
  
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

Procedure attack(*fighter.Fighter, attack.b)
  ClearList(*fighter\fightersHit())
  setState(*fighter, #STATE_ATTACK, attack)
EndProcedure

;système d'acceptable animations

Procedure updateAnimations(*game.Game)
  Shared stateDefaultAnimation(), commandDefaultAnimation()
  Define animation.s, *fighter.Fighter
  ForEach *game\fighters()
    *fighter = @*game\fighters()
    If *fighter\updateAnim
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
    *fighter\updateAnim = 0
  Next 
EndProcedure

Procedure defaultJumpAnimCallback(*fighter.Fighter, *data)
  *fighter\updateAnim = 1
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

Procedure testRectCollision(x1.l, y1.l, w1.l, h1.l, x2.l, y2.l, w2.l, h2.l)
  If x1 < x2 + w2 And
     x1 + w1 > x2 And
     y1 > y2 - h2 And
     y1 - h1 < y2
    ProcedureReturn 1
  EndIf 
EndProcedure 

Procedure.d getKnockback(damage.d)
  ProcedureReturn damage / 2
EndProcedure

Procedure getHitlag(damage.d)
  ProcedureReturn (damage * 0.65) + 6
EndProcedure

Procedure getHitstun(knockback.b)
  ProcedureReturn knockback * 5
EndProcedure 

;fonctionnement de la hitstun/tumble
; - à la fin de la durée de la hitstun, les persos passent soient en IDLE, soient en TUMBLE
; - ses changement de state sont indépendants de l'animation (un perso peut rester en anim de hitstun même s'il est en IDLE indéfiniment, jusqu'à changer son anim lui-même)
; - un perso en heavy hitsutn passe en anim de tumble quand la hitstun est finie ET qu'il descend

Procedure startKnockback(*hitbox.Hitbox, *hurtbox.Hurtbox, *attacking.Fighter, *defending.Fighter)
  Define hitlag.b, type.b, hitstun.l, anim.s, facing.b
  If *attacking\facing = 1
    angle = *hitbox\angle
  Else
    angle = 180 - *hitbox\angle
  EndIf
  angle = Radian(angle)
  knockback = getKnockback(*hitbox\damage)
  *defending\physics\v\x = Cos(angle) * knockback
  *defending\physics\v\y = Sin(angle) * knockback
  If knockback > 100
    type = #KB_TUMBLE
    anim = "hurtheavy"
  Else
    anim = "hurt"
  EndIf 
  If *defending\physics\v\y < 2 And *defending\grounded
    *defending\physics\v\y = 0
    anim = "hurtground"
  EndIf
  facing = -Sign(*defending\physics\v\x)
  
  hitlag = getHitlag(*hitbox\damage)
  *defending\paused = hitlag
  *attacking\paused = hitlag
  hitstun = getHitstun(knockback)
  Debug hitstun
  setAnimation(*defending, anim, 0, facing)
  setState(*defending, #STATE_HITSTUN, type + (hitstun << 1))
EndProcedure

Procedure hit(*hitbox.Hitbox, *hurtbox.Hurtbox, *attacking.Fighter, *defending.Fighter)
  Define angle.d, knockback.d
  
  ForEach *attacking\fightersHit()
    If *attacking\fightersHit()\fighter = *defending And *attacking\fightersHit()\hitID = *hitbox\hit
      ProcedureReturn 0
    EndIf 
  Next
  
  Select *defending\state
    Case #STATE_GUARD  
      ;shieldstun
      AddElement(*attacking\fightersHit())
      *attacking\fightersHit()\hitID = *hitbox\hit
      *attacking\fightersHit()\fighter = *defending
    Case #STATE_CUSTOM
      ;custom state handling
    Default
      AddElement(*attacking\fightersHit())  
      *attacking\fightersHit()\hitID = *hitbox\hit
      *attacking\fightersHit()\fighter = *defending
      startKnockback(*hitbox, *hurtbox, *attacking, *defending)
    EndSelect
EndProcedure

Procedure manageHitboxes(*game.Game)
  Define *attacking.Fighter, *defending.Fighter, *hitbox.Hitbox, *hurtbox.Hurtbox
  bgc = #White
  ForEach *game\fighters()
    *attacking = @*game\fighters()
    ForEach  *game\fighters()
      *defending = @*game\fighters()
      If *defending = *attacking
        Continue
      EndIf 
      ForEach *attacking\currentAnimation\frames()\model\hitboxes()
        ForEach *defending\currentAnimation\frames()\model\hurtboxes()
          *hitbox = @*attacking\currentAnimation\frames()\model\hitboxes()
          *hurtbox = @*defending\currentAnimation\frames()\model\hurtboxes()
          If testRectCollision(getRealCboxX(*hitbox, *attacking\facing) + *attacking\x, *attacking\y + *hitbox\y, *hitbox\x2, *hitbox\y2,
                               getRealCboxX(*hurtbox, *defending\facing) + *defending\x, *defending\y + *hurtbox\y, *hurtbox\x2, *hurtbox\y2)
            hit(*hitbox, *hurtbox, *attacking, *defending)
            bgc = #Black
          EndIf 
        Next
      Next 
    Next
    ChangeCurrentElement(*game\fighters(), *attacking)  
  Next  
EndProcedure


; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 407
; FirstLine = 389
; Folding = ------
; EnableXP
; SubSystem = OpenGL
; EnableUnicode