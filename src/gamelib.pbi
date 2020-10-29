frame.l = 0
Global bgc.l = #White
Global testVal.b = 0

;- Structures

Structure Frame
  *model.FrameModel
  timeLeft.d
EndStructure  

Structure Animation
  *model.AnimationModel
  List frames.Frame()
  frame.b
  speed.d
  frameMultiplier.b
  carry.f ;for animations with speed
  currentCarry.f
  facing.b
  onFrameChanged.f_callback
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
  currentMove.MoveInfo
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
  shieldSize.d
  List fightersHit.fighterHitIdPair()
EndStructure

Structure Platform
  *animation.Animation
  *model.PlatformModel
EndStructure

Structure Stage
  *model.StageModel
  *backgroundAnim.Animation
  List platforms.Platform()
EndStructure 

Structure Camera Extends Vector
EndStructure

Structure Game
  List fighters.Fighter()
  *currentStage.Stage
  window.l
  camera.Camera
EndStructure

Define frameRate.b = 60, frameDuration.f

;- Rendering objects
CreateImage(0, #SCREEN_W, #SCREEN_H, 32, #PB_Image_Transparent)

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
  *game\camera\x = 0
  *game\camera\y = 0
  ProcedureReturn *game
EndProcedure
 
Procedure resetAnimation(*animation.Animation)  
  *animation\frame = 0
  ResetList(*animation\frames())
  NextElement(*animation\frames())
  If *animation\frameMultiplier > 1
    *animation\currentCarry = 0
    *animation\frames()\timeLeft = *animation\frameMultiplier
  ElseIf *animation\frames()\model\duration
    *animation\frames()\timeLeft = *animation\frames()\model\duration
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
  ElseIf *anim\speed = -1
    ProcedureReturn 0
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
  *game\currentStage\model = *model
  If *game\currentStage\model\backgroundAnim
    *game\currentStage\backgroundAnim = AllocateStructure(Animation)
    initAnimation(*game\currentStage\backgroundAnim, *game\currentStage\model\backgroundAnim)
  EndIf 
  ForEach *model\platforms()
    AddElement(*game\currentStage\platforms())
    *game\currentStage\platforms()\model = *model\platforms()
    If *model\platforms()\animationName
      *game\currentStage\platforms()\animation = AllocateStructure(Animation)
      initAnimation(*game\currentStage\platforms()\animation, getStageAnimation(*model, *model\platforms()\animationName))
      setAnimationSpeed(*game\currentStage\platforms()\animation)
    EndIf 
  Next 
  *game\camera\x = (*game\currentStage\model\w - #SCREEN_W) / 2
EndProcedure 

Procedure setAnimation(*fighter.Fighter, name.s, speed.d = 0, facing = 0, reset.b = 0)
  Define *anim.Animation
  If *fighter\currentAnimationName = name And Not reset
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
  *fighter\currentAnimation\frame = 0
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

Procedure drawAnimationFrame(*frame.FrameModel, spriteSheet.l, x.l, y.l, facing = 1)
  Define dx.l
  With *frame
    If facing = 1
      dx = x - \origin\x
    Else
      dx = x + \origin\x - \display\w
    EndIf
    ClipSprite(spriteSheet, \display\x, \display\y, \display\w, \display\h)
    DisplayTransparentSprite(spriteSheet, dx, y - \origin\y)
  EndWith
EndProcedure

Procedure renderShield(*fighter.Fighter, *camera, x.l, y.l)
  *info.ShieldInfo = *fighter\character\shieldInfo
  DrawingMode(#PB_2DDrawing_AlphaBlend)
  Circle( x + *info\x,  y - *info\y, *info\size * *fighter\shieldSize, RGBA(255,   0,   0, 96))
EndProcedure

Procedure renderFighter(*fighter.Fighter, *camera.Camera)
  Define spriteSheet.l, facing.b, x.l, y.l
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
  *frame.FrameModel = *fighter\currentAnimation\frames()\model
  x = *fighter\x - *camera\x
  y = #SCREEN_H - (*fighter\y - *camera\y)
  drawAnimationFrame(*frame, spriteSheet, x, y, facing)
  
  ;direct drawing
  StartDrawing(ImageOutput(0)) 
  If *fighter\state = #STATE_GUARD
    renderShield(*fighter, *camera, x, y)
  EndIf 
  CompilerIf #DEBUG 
    Define hurtColor.l
    If *fighter\state = #STATE_HITSTUN
      hurtColor = #YellowAlpha
    Else
      hurtColor = #GreenAlpha
    EndIf 
    DrawingMode(#PB_2DDrawing_AllChannels | #PB_2DDrawing_Outlined)
    Line(0, y, #SCREEN_W, 1, #BlueAlpha)
    ForEach *frame\hitboxes()
      Box(x + getRealCboxX(*frame\hitboxes(), facing), y - *frame\hitboxes()\y, *frame\hitboxes()\x2, *frame\hitboxes()\y2, #RedAlpha)
    Next
    ForEach *frame\hurtboxes()
      Box(x + getRealCboxX(*frame\hurtboxes(), facing), y - *frame\hurtboxes()\y, *frame\hurtboxes()\x2, *frame\hurtboxes()\y2, hurtColor)
    Next
  CompilerEndIf
  StopDrawing()
EndProcedure

Procedure renderPlatform(*platform.Platform, *camera.Camera)
  drawAnimationFrame(*platform\animation\frames()\model, *platform\animation\model\spriteSheet, *platform\model\x - *camera\x, #SCREEN_H - (*platform\model\y - *camera\y))
EndProcedure

Procedure renderStage(*stage.Stage)
  If *stage\backgroundAnim
    DisplayTransparentSprite(*stage\backgroundAnim\model\spriteSheet, 0, 0)
  EndIf 
EndProcedure

Procedure renderFrame(*game.Game)
  Define medPos.Vector
  ClearScreen(bgc)
  
  ForEach *game\fighters()
    medPos\x + *game\fighters()\x 
    medPos\y + *game\fighters()\y
  Next 
  medPos\x / ListSize(*game\fighters())
  medPos\x - (#SCREEN_W / 2)
  
  medPos\y / ListSize(*game\fighters())
  medPos\y - (#SCREEN_H / 2)
  
  If Abs (*game\camera\x - medPos\x) < kuribrawl\variables\cameraMaxSpeed
    *game\camera\x = medPos\x
  Else
    *game\camera\x + (kuribrawl\variables\cameraMaxSpeed * -Sign(*game\camera\x - medPos\x))
  EndIf 
  
  If *game\camera\x < *game\currentStage\model\cameraZone\left
    *game\camera\x = *game\currentStage\model\cameraZone\left
  ElseIf *game\camera\x + #SCREEN_W > *game\currentStage\model\cameraZone\right
    *game\camera\x =  *game\currentStage\model\cameraZone\right - #SCREEN_W
  EndIf 
  
  If Abs (*game\camera\y - medPos\y) < kuribrawl\variables\cameraMaxSpeed
    *game\camera\y = medPos\y
  Else
    *game\camera\y + (kuribrawl\variables\cameraMaxSpeed * -Sign(*game\camera\y - medPos\y))
  EndIf 
  
  If *game\camera\y < *game\currentStage\model\cameraZone\bottom
    *game\camera\y = *game\currentStage\model\cameraZone\bottom
  ElseIf *game\camera\y + #SCREEN_H > *game\currentStage\model\cameraZone\top 
    *game\camera\y =  *game\currentStage\model\cameraZone\top - #SCREEN_H
  EndIf 
  
  If *game\currentStage
    renderStage(*game\currentStage)
  EndIf 
  ForEach *game\currentStage\platforms()
    renderPlatform(@*game\currentStage\platforms(), *game\camera)
  Next
  
  ForEach *game\fighters()
    renderFighter(@*game\fighters(), *game\camera)
  Next
  
  StartDrawing(ScreenOutput())
  DrawAlphaImage(ImageID(0), 0, 0)
  StopDrawing()
  StartDrawing(ImageOutput(0))
  DrawingMode(#PB_2DDrawing_AlphaChannel)
  Box(0, 0, #SCREEN_W, #SCREEN_H, $00000000)
  StopDrawing()
  
  FlipBuffers()
  bgc = #White
EndProcedure

Procedure applyFrameMovement(*fighter.Fighter, *frame.FrameModel)
  Define add.b = 1 - ((*frame\speedMode & %10) >> 1)
  If Not *frame\speedMode & %1000
    *fighter\physics\v\x = *frame\speed\x + (add * *fighter\physics\v\x)
  EndIf 
  If Not *frame\speedMode & %10000
    *fighter\physics\v\y = *frame\speed\y + (add * *fighter\physics\v\y)
  EndIf 
EndProcedure

;avancer les anims des stages (bg/plat)
Procedure nextFrame(*animation.Animation, *fighter.Fighter) 
  *animation\frame + 1
  If NextElement(*animation\frames()) = 0
    If *animation\endCallback
      If *animation\endCallback = 1
        ProcedureReturn
      EndIf 
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
  If *animation\onFrameChanged
    *animation\onFrameChanged(*fighter, *animation\frames())
  EndIf 
  If *animation\frames()\model\speedMode & 1 And Not (*animation\frames() & %100 )
    applyFrameMovement(*fighter, *animation\frames()\model)
  EndIf 
EndProcedure

Procedure skipToFrame(*animation.Animation, *fighter.Fighter, frame.b) ; not compatible with animations with anim modifiers. or maybe it is. idk
  While *animation\frame < frame
    *animation\currentCarry + *animation\carry
    nextFrame(*animation, *fighter)
  Wend
EndProcedure

Procedure advanceAnimations(*game.Game)
  Define *fighter.Fighter
  ForEach *game\fighters()
    *fighter = *game\fighters()
    If *fighter\paused > 0
      ProcedureReturn 0
    EndIf
    If *fighter\currentAnimation\frames()\timeLeft >= 1
      *fighter\currentAnimation\frames()\timeLeft - 1
    ElseIf Not *fighter\currentAnimation\speed = -1
      *fighter\currentAnimation\currentCarry + *fighter\currentAnimation\carry
      nextFrame(*fighter\currentAnimation, *fighter)
      ProcedureReturn 1
    EndIf 
    If *fighter\currentAnimation\frames()\model\speedMode & %101
      applyFrameMovement(*fighter, *fighter\currentAnimation\frames()\model)
    EndIf  
  Next
EndProcedure

Procedure isFighterGrounded(*fighter.Fighter)
  ProcedureReturn *fighter\grounded
EndProcedure

Procedure figherDirection(*fighter.Fighter)
  ProcedureReturn *fighter\facing
EndProcedure

Procedure getCurrentAttack(*fighter.Fighter)
  ProcedureReturn *fighter\stateInfo & %11111
EndProcedure

Procedure getCurrentAttackInfo(*fighter.Fighter)
  ProcedureReturn *fighter\character\moves(getCurrentAttack(*fighter))
EndProcedure

CompilerIf #DEBUG
  Declare logState(state.b, facing.b = 0, previousTime.u = 0)
CompilerEndIf
Procedure setState(*fighter.Fighter, state.b, info.l = 0, updateAnim.b = 1)
  *fighter\state = state
  *fighter\stateInfo = info
  CompilerIf #DEBUG
    logState(state, *fighter\facing, *fighter\stateTimer)
  CompilerEndIf
  *fighter\stateTimer = 0
  *fighter\updateAnim = updateAnim
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
      setAnimation(*fighter, "jump")
    Case #YJUMP_NORMAL
      *fighter\physics\v\y = *fighter\character\jumpSpeed
      setAnimation(*fighter, "jump")
    Case #YJUMP_DOUBLE
      *fighter\physics\v\y = *fighter\character\doubleJumpSpeed
      setAnimation(*fighter, "doublejump", 0, 0, 1)
  EndSelect
  setState(*fighter, #STATE_IDLE, 1, 0)
EndProcedure

Procedure crouch(*fighter.Fighter)
  setState(*fighter, #STATE_CROUCH_START)
EndProcedure

Declare multiMoveFrameCallback(*fighter.Fighter, *frame.FrameModel)
Procedure attack(*fighter.Fighter, attack.b)
  Shared commandDefaultAnimation()
  ClearList(*fighter\fightersHit())
  setState(*fighter, #STATE_ATTACK, attack + (elementType << 8) + (element << 10))
  setAnimation(*fighter, commandDefaultAnimation(attack))
  *fighter\currentMove = *fighter\character\moves(attack)
  If *fighter\currentMove\multiMove
    *fighter\currentAnimation\onFrameChanged = @multiMoveFrameCallback()
  EndIf
  testVal = 1
EndProcedure

Procedure endAttack(*fighter)
  Define attack.b = getCurrentAttack(*fighter)
  setState(*fighter, #STATE_IDLE)
EndProcedure

Procedure defaultJumpAnimCallback(*fighter.Fighter, *data)
  *fighter\updateAnim = 1
EndProcedure

Procedure defaultAttackAnimCallback(*fighter.Fighter, *data)
  endAttack(*fighter)
EndProcedure

Procedure multiMoveFrameCallback(*fighter.Fighter, *frame.FrameModel)
  Define part.b = (*fighter\stateInfo & %11100000) >> 5
  If part > 0 And *fighter\currentAnimation\frame = *fighter\currentMove\multiMove\partEndFrames(part - 1)
    skipToFrame(*fighter\currentAnimation, *fighter, *fighter\currentMove\multiMove\partStartFrames(part - 1))
  ElseIf part <= ArraySize(*fighter\currentMove\multiMove\partStartFrames()) And *fighter\currentAnimation\frame = *fighter\currentMove\multiMove\partStartFrames(part)
    endAttack(*fighter)
  EndIf 
EndProcedure

;système d'acceptable animations
Procedure updateAnimations(*game.Game)
  Shared stateDefaultAnimation()
  Define animation.s, *fighter.Fighter
  ForEach *game\fighters()
    *fighter = @*game\fighters()
    
    If *fighter\state = #STATE_TUMBLE And *fighter\physics\v\y < 0
      setAnimation(*fighter, "tumble")
    EndIf 
    
    If *fighter\updateAnim
      ;case by case
      Select *fighter\state
        Case #STATE_IDLE
          If *fighter\grounded = 0
              setAnimation(*fighter, "airIdle")
          ElseIf *fighter\grounded = 1
            setAnimation(*fighter, "idle")
          EndIf 
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

Procedure initFighters(*game.Game)
  ForEach *game\fighters()
    *game\fighters()\shieldSize = 1.0
    *game\fighters()\grounded = 0
    setState(*game\fighters(), #STATE_IDLE, 0)
  Next 
  updateAnimations(*game)
EndProcedure

Procedure.d getShieldDecrement(currentShield.d)
  Debug "oui"
  Debug currentShield
  Debug Exp((-Pow(currentShield - 0.7, 2)) * 15) / 20
  ProcedureReturn Exp(-(Sqr(currentShield) - 0.7) * 15) / 20
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
  ProcedureReturn damage
EndProcedure

Procedure getHitlag(damage.d)
  ProcedureReturn (damage * 0.65) + 6
EndProcedure

Procedure getHitstun(knockback.b)
  ProcedureReturn knockback * 2
EndProcedure 

;fonctionnement de la hitstun/tumble
; - à la fin de la durée de la hitstun, les persos passent soient en IDLE, soient en TUMBLE
; - ses changement de state sont indépendants de l'animation (un perso peut rester en anim de hitstun même s'il est en IDLE indéfiniment, jusqu'à changer son anim lui-même)
; - un perso en heavy hitsutn passe en anim de tumble quand la hitstun est finie ET qu'il descend

Procedure startKnockback(*hitbox.Hitbox, *hurtbox.Hurtbox, *attacking.Fighter, *defending.Fighter)
  Define hitlag.b, type.b, hitstun.l, anim.s, facing.b, angle.d
  If *attacking\facing = 1
    degAngle.l = *hitbox\angle
  Else
    degAngle.l = 180 - *hitbox\angle
  EndIf
  angle = Radian(degAngle)
  
  If *defending\state = #STATE_HITSTUN
    Debug "TRUE"
  EndIf 
  
  knockback = getKnockback(*hitbox\damage)
  *defending\physics\v\x = Cos(angle) * knockback
  *defending\physics\v\y = Sin(angle) * knockback
  
  facing = -Sign(*defending\physics\v\x)
  If knockback > 10
    type = #KB_TUMBLE
    If degAngle > 60 And degAngle < 120 And *attacking\y < *defending\y
      anim = "hurtup"
      facing = 0
    ElseIf degAngle > 240 And degAngle < 300
      anim = "hurtdown"
      facing = 0
    Else
      anim = "hurtheavy"
    EndIf   
  Else
    anim = "hurt"
  EndIf 
  If *defending\physics\v\y < 2 And *defending\grounded And Not type = #KB_TUMBLE
    *defending\physics\v\y = 0
    anim = "hurtground"
  EndIf

  
  hitlag = getHitlag(*hitbox\damage)
  *defending\paused = hitlag
  *attacking\paused = hitlag
  hitstun = getHitstun(knockback)
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

Procedure death(*fighter.Fighter, *game.Game)
  *fighter\x = *game\currentStage\model\w / 2
  *fighter\y = 500
  *fighter\physics\v\x = 0
  *fighter\physics\v\y = 0
  setState(*fighter, #STATE_IDLE)
EndProcedure

Procedure manageHitboxes(*game.Game)
  Define *attacking.Fighter, *defending.Fighter, *hitbox.Hitbox, *hurtbox.Hurtbox, *successfulHitbox.Hitbox
  ForEach *game\fighters()
    *attacking = @*game\fighters()
    ForEach  *game\fighters()
      *defending = @*game\fighters()
      If *defending = *attacking
        Continue
      EndIf 
      *successfulHitbox = 0
      ForEach *attacking\currentAnimation\frames()\model\hitboxes()
        ForEach *defending\currentAnimation\frames()\model\hurtboxes()
          *hitbox = @*attacking\currentAnimation\frames()\model\hitboxes()
          *hurtbox = @*defending\currentAnimation\frames()\model\hurtboxes()
          If testRectCollision(getRealCboxX(*hitbox, *attacking\facing) + *attacking\x, *attacking\y + *hitbox\y, *hitbox\x2, *hitbox\y2,
                               getRealCboxX(*hurtbox, *defending\facing) + *defending\x, *defending\y + *hurtbox\y, *hurtbox\x2, *hurtbox\y2)
            bgc = #Black
            If Not *successfulHitbox Or *hitbox\priority > *successfulHitbox\priority
              *successfulHitbox = *hitbox
            EndIf 
          EndIf 
        Next
      Next
      If *successfulHitbox 
        hit(*successfulHitbox, *hurtbox, *attacking, *defending)
      EndIf 
    Next
    ChangeCurrentElement(*game\fighters(), *attacking)  
  Next  
EndProcedure

Declare freeGame(*game.Game)
Procedure endGame(*game.Game)
  freeGame(*game)
EndProcedure

Procedure updateFrameRate()
  Shared frameRate, frameDuration
  frameDuration.f = 1000 / frameRate
  Debug "New framerate " + Str(frameRate)
EndProcedure  
  
Procedure lowerFrameRate()
  Shared frameRate
  frameRate - 5
  updateFrameRate()
EndProcedure
  
Procedure increaseFrameRate()
  Shared frameRate
  frameRate + 5
  updateFrameRate()
EndProcedure
  

; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 554
; FirstLine = 519
; Folding = --0-4----
; EnableXP
; SubSystem = OpenGL
; EnableUnicode