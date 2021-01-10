frame.l = 0
Global bgc.l = #White
Global testVal.b = 0

;- Structures

Structure fighterHitIdPair
  *fighter
  hitID.b
EndStructure

Structure Fighter
  *character.Champion ;base character
  List sprites.Sprite()
  x.l ;position of its origin (relative to stage center, upward Y)
  y.l
  physics.Physics 
  currentAnimation.Animation ;on pourrait s'en passer mais avoir un ptr vers l'objet évite de faire un accès map à chaque fois
  currentAnimationName.s
  currentMove.MoveInfo ;infos on the current move (not reset at the end of the move, thus can point to a move even when not attacking)
  *port                ;inputLib port
  portID.b
  name.s ;identification name (mainly for debug purposes)
  grounded.b ;whether the fighter is on the ground or not
  facing.b ;1 = right | -1 = left 
  state.b 
  stateInfo.l ;attack, direction, input that was used to start this state, etc
  stateTimer.u ;number of frame since the state was set
  updateAnim.b ;"should update anim" : generally wheter state has been changed since last animation change (sometimes state is changed without updating the anim)
  jumps.b      ;remaining aerial jumps
  damage.d
  paused.b ;indicates freeze state (for hitlag)
  shieldSize.d ;multiplier of the actual shield size (btw 0 and 1)
  shieldPosition.Vector ;current pos of the shield (changed by shield tilting)
  List fightersHit.fighterHitIdPair() ;hits that have connected in the current move (to avoid hitting the same fighter twice with the same hit)
EndStructure

Structure Platform
  animation.Animation
  *model.PlatformModel
EndStructure

Structure Stage
  *model.StageModel
  backgroundAnim.Animation
  List platforms.Platform()
EndStructure 

Structure Game
  List fighters.Fighter()
  *currentStage.Stage
  window.l
  camera.Camera
  *base.GameData
EndStructure

Define frameRate.b = 60, frameDuration.f

;- Object initialization ---

Procedure initGame(window.l)
  *game.Game = AllocateStructure(Game)
  *game\window = window
  *game\camera\x = 0
  *game\camera\y = 0
  *game\base = kuribrawl
  ProcedureReturn *game
EndProcedure

Procedure newFighter(*game.Game, *character.Champion, x.l, y.l, port = -1)
  
  Define *anim.Animation, *model.AnimationModel
  *r.Fighter = AddElement(*game\fighters())
  *r\character = *character
  *r\x = x
  *r\y = y
  *r\physics\v\x = 0
  *r\physics\v\y = 0
  *r\physics\a\x = 0
  *r\physics\a\y = 0
  
  initAnimation(*r\currentAnimation, getAnimation(*character, "idle"))  ;TODO make that fighters don't need to always have a current animation
  
  *r\state = #STATE_IDLE
  
  *r\portID = port
  ProcedureReturn *r
EndProcedure

Procedure setStage(*game.Game, *model.StageModel) ;actually creates a stage and sets it as the current stage of the game
  Define *model.StageModel, *animModel.AnimationModel
  If Not *model
    ProcedureReturn 0
  EndIf 
  *game\currentStage = AllocateStructure(Stage) ;creating the stage structure
  *game\currentStage\model = *model 
  
  If *game\currentStage\model\backgroundAnim  ;if the stage has a background animation
    initAnimation(*game\currentStage\backgroundAnim, *game\currentStage\model\backgroundAnim) ;initalizes it as its current animation
  EndIf 
  ForEach *model\platforms()  ;initializing the platforms
    AddElement(*game\currentStage\platforms())
    *game\currentStage\platforms()\model = *model\platforms() 
    If *model\platforms()\animationName ;if the platform has an animation
      *animModel = getStageAnimation(*model, *model\platforms()\animationName)  ;we get it by name in the stage's animations
      If Not *animModel
        error("Can't set animation platform (animation " + *model\platforms()\animationName + " was not found in " + *model\name + ".)")
      EndIf 
      
      initAnimation(*game\currentStage\platforms()\animation, *animModel) ;and we init is as the current platform animation
    EndIf 
  Next 
  *game\camera\x = (*game\currentStage\model\w - #SCREEN_W) / 2
EndProcedure 

;- Fighter methods ---

Procedure setAnimation(*fighter.Fighter, name.s, speed.d = 0, facing = 0, reset.b = 0)  ;sets the current animation of a fighter
  Define *model.AnimationModel
  
  If *fighter\currentAnimationName = name And Not reset
    ProcedureReturn 0
  EndIf
  
  *model = getAnimation(*fighter\character, name)
  If Not *model
    Debug "Animation " + name + " not found. (in character " + *fighter\character\name + ".)"
  EndIf
  
  *fighter\currentAnimationName = name
  setAnimationModel(*fighter\currentAnimation, *model, speed)
EndProcedure

Procedure setAnimationState(*fighter.Fighter, speed.d = 0, reset.b = 0) ;sets the current animation according to the state
  Shared stateDefaultAnimation()
  animation.s = stateDefaultAnimation(*fighter\state)
  setAnimation(*fighter, animation, speed, 0, reset)
EndProcedure

Procedure addFighterSprite(*fighter.Fighter, x.l = 0, y.l = 0)
  *sprite.Sprite = AddElement(*fighter\sprites())
  *sprite\pos\x = x
  *sprite\pos\y = y
  ProcedureReturn *sprite
EndProcedure

Procedure removeFighterSprite(*fighter.Fighter, *sprite)
  ChangeCurrentElement(*fighter\sprites(), *sprite)
  DeleteElement(*fighter\sprites())
EndProcedure

;- Fighter getters ---

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

Procedure.d getShieldDecrement(currentShield.d)
  ProcedureReturn kuribrawl\variables\shieldDecay
EndProcedure

Procedure.d getShieldTiltFactor(*fighter.Fighter)
  ProcedureReturn *fighter\character\shieldInfo\size * (1 - *fighter\shieldSize)
EndProcedure

Procedure getActualShieldX(*fighter.Fighter)
  ProcedureReturn  *fighter\x + *fighter\character\shieldInfo\x + *fighter\shieldPosition\x
EndProcedure

Procedure getActualShieldY(*fighter.Fighter)
  ProcedureReturn  *fighter\y + *fighter\character\shieldInfo\y + *fighter\shieldPosition\y
EndProcedure

Procedure getRealCboxX(*cbox.CollisionBox, facing)
  If facing = 1
    ProcedureReturn *cbox\x
  Else
    ProcedureReturn - *cbox\x - *cbox\x2
  EndIf 
EndProcedure

;- Fighter state ---

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
  *fighter\grounded = 0
  setState(*fighter, #STATE_IDLE, 1, 0)
EndProcedure

Procedure crouch(*fighter.Fighter)
  setState(*fighter, #STATE_CROUCH_START)
EndProcedure

Declare multiMoveFrameCallback(*fighter.Fighter, *frame.Frame)
Procedure attack(*fighter.Fighter, attack.b)
  Shared commandDefaultAnimation()
  ClearList(*fighter\fightersHit())
  setState(*fighter, #STATE_ATTACK, attack + (elementType << 8) + (element << 10), 0)
  setAnimation(*fighter, commandDefaultAnimation(attack))
  *fighter\currentMove = *fighter\character\moves(attack)
  If *fighter\currentMove\multiMove
    *fighter\currentAnimation\onFrameChanged = @multiMoveFrameCallback()
  EndIf
EndProcedure

Procedure endAttack(*fighter)
  Define attack.b = getCurrentAttack(*fighter)
  setState(*fighter, #STATE_IDLE)
EndProcedure

Procedure death(*fighter.Fighter, *game.Game)
  *fighter\x = *game\currentStage\model\w / 2
  *fighter\y = 500
  *fighter\physics\v\x = 0
  *fighter\physics\v\y = 0
  setState(*fighter, #STATE_IDLE)
  
  *fighter\damage = 0
EndProcedure

;- Rendering ---

Procedure renderHUD(*game.Game)
  Define x.l, y.l
  y = 20
  ForEach *game\fighters()
    x = DrawDamageText(kuribrawl\HUD\damageFont, Int(*game\fighters()\damage), 20, y)
    DisplaySprite(*game\fighters()\character\assets\HUDIcon, x, y)
    y + 50
  Next
EndProcedure

Procedure renderShield(*fighter.Fighter, *camera, x.l, y.l)
  *info.ShieldInfo = *fighter\character\shieldInfo
  DrawingMode(#PB_2DDrawing_AlphaBlend)
  Circle( x + *info\x + *fighter\shieldPosition\x,  y - *info\y + *fighter\shieldPosition\y, *info\size * *fighter\shieldSize, shieldColor)
EndProcedure

Procedure renderFighter(*fighter.Fighter, *camera.Camera)
  Define facing.b, x.l, y.l, *sprite.Sprite
  
  If *fighter\currentAnimation\facing = 0
    facing = *fighter\facing
  Else
    facing = *fighter\currentAnimation\facing
  EndIf 
  
  x = *fighter\x - *camera\x
  y = #SCREEN_H - (*fighter\y - *camera\y)
  
  *frame.Frame = renderAnimation(*fighter\currentAnimation, *camera, x, y, Bool(facing = -1))
  
  If ListSize(*fighter\sprites()) 
    ForEach *fighter\sprites()
      *sprite = @*fighter\sprites()
      
      renderAnimation(*sprite\animation, *camera, x + *sprite\pos\x, y - *sprite\pos\y)
    Next
  EndIf 
  
  ;direct drawing
  
  StartDrawing(ImageOutput(0)) 
  If *fighter\state = #STATE_GUARD Or *fighter\state = #STATE_GUARDSTUN
    renderShield(*fighter, *camera, x, y)
  EndIf 
  CompilerIf #DEBUG 
    Define hurtColor.l
    If *fighter\state = #STATE_HITSTUN Or *fighter\state = #STATE_GUARDSTUN
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
  drawAnimationFrame(getCurrentFrame(*platform\animation), *platform\animation\model\spriteSheet, *platform\model\x - *camera\x, #SCREEN_H - (*platform\model\y - *camera\y))
EndProcedure

;TODO faire réellement fonctionner l'anim du stage
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
  StartDrawing(ScreenOutput()) ;takes between 4 and 6 ms
  DrawAlphaImage(ImageID(0), 0, 0)
  StopDrawing()
  StartDrawing(ImageOutput(0)) ;takes between 0 and 1 ms
  DrawingMode(#PB_2DDrawing_AlphaChannel)
  Box(0, 0, #SCREEN_W, #SCREEN_H, $00000000)
  StopDrawing()
  
  renderHUD(*game)
  
  FlipBuffers()
  bgc = #White
EndProcedure

;- Game animations ---

Procedure advanceAnimations(*game.Game)
  Define *fighter.Fighter
  ForEach *game\fighters()
    *fighter = *game\fighters()
    If *fighter\paused > 0
      ProcedureReturn 0
    EndIf
    advanceAnimation(*fighter\currentAnimation, *fighter)
    *frame.Frame = getCurrentFrame(*fighter\currentAnimation)
    If *frame\speedMode & %101
      applyFrameMovement(*fighter, *frame)
    EndIf  
    
    If ListSize(*fighter\sprites())
      ForEach *fighter\sprites()
        advanceAnimation(*fighter\sprites()\animation)
      Next
    EndIf
  Next
EndProcedure

Procedure applyFrameMovement(*fighter.Fighter, *frame.Frame)
  Define add.b = 1 - ((*frame\speedMode & %10) >> 1)
  If Not *frame\speedMode & %1000
    *fighter\physics\v\x = *frame\speed\x + (add * *fighter\physics\v\x)
  EndIf 
  If Not *frame\speedMode & %10000
    *fighter\physics\v\y = *frame\speed\y + (add * *fighter\physics\v\y)
  EndIf 
EndProcedure

Procedure defaultJumpAnimCallback(*fighter.Fighter, *data)
  *fighter\updateAnim = 1
EndProcedure

Procedure defaultAttackAnimCallback(*fighter.Fighter, *data)
  endAttack(*fighter)
EndProcedure

Procedure multiMoveFrameCallback(*fighter.Fighter, *frame.Frame)
  Define part.b = (*fighter\stateInfo & %11100000) >> 5
  If part > 0 And *fighter\currentAnimation\currentFrame = *fighter\currentMove\multiMove\partEndFrames(part - 1)
    skipToFrame(*fighter\currentAnimation, *fighter\currentMove\multiMove\partStartFrames(part - 1))
  ElseIf part <= ArraySize(*fighter\currentMove\multiMove\partStartFrames()) And *fighter\currentAnimation\currentFrame = *fighter\currentMove\multiMove\partStartFrames(part)
    endAttack(*fighter)
  EndIf 
EndProcedure

;système d'acceptable animations
Procedure updateAnimations(*game.Game)
  Define *fighter.Fighter
  ForEach *game\fighters()
    *fighter = @*game\fighters()
    
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
          setAnimationState(*fighter, 0, *fighter\updateAnim & %10)
      EndSelect
      If *fighter\state = #STATE_LANDING_LAG
        setAnimationSpeed(*fighter\currentAnimation, *fighter\stateInfo)
      EndIf
    ElseIf *fighter\state = #STATE_TUMBLE And *fighter\physics\v\y < 0 And Not *fighter\currentAnimationName = "tumble"
      setAnimation(*fighter, "tumble")
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

;- Hits ---

Procedure testRectCollision(x1.l, y1.l, w1.l, h1.l, x2.l, y2.l, w2.l, h2.l)
  If x1 < x2 + w2 And
     x1 + w1 > x2 And
     y1 > y2 - h2 And
     y1 - h1 < y2
    ProcedureReturn 1
  EndIf 
EndProcedure 

Procedure testRectCircleCollision(rx.l, ry.l, rw.l, rh.l, cx.l, cy.l, cr.l)
  Define testPos.Vector
  Define testDist.Vector
  Define distance.l
  testPos\x = cx
  testPos\y = cy
  If cx < rx 
    testPos\x = rx ;left
  ElseIf cx > rx + rw
    testPos\x = rx + rw ;right
  EndIf 
  
  If cy < ry 
    testPos\y = ry ;above
  ElseIf cy > ry + rh
    testPos\y = ry + rh ;below
  EndIf 
  
  testDist\x = cx - testPos\x
  testDist\y = cy - testPos\y
  distance = Int(Sqr((testDist\x * testDist\x) + (testDist\y * testDist\y)))
  
  If distance <= cr
    ProcedureReturn 1
  EndIf
  
  ProcedureReturn 0
EndProcedure

Procedure getKnockback(percentage.l, bkb.f, skb.f, weight.d)
  ProcedureReturn bkb + (((percentage * weight) + kuribrawl\variables\knockbackBase) * skb * kuribrawl\variables\knockbackMult)
EndProcedure

Procedure getHitlag(damage.d)
  ProcedureReturn (damage * 0.5) + 2
EndProcedure

Procedure getHitstun(knockback.b)
  ProcedureReturn knockback * 2
EndProcedure 

Procedure getShieldKnockback(percentage.l, bkb.f, skb.f, weight.d)
  ProcedureReturn 5
EndProcedure

Procedure getShieldStun(knockback.b)
  ProcedureReturn knockback * 5
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
  
  *defending\damage + *hitbox\damage
  
  knockback = getKnockback(*defending\damage, *hitbox\bkb, *hitbox\skb, *defending\character\weight)
  *defending\physics\v\x = Cos(angle) * knockback
  *defending\physics\v\y = Sin(angle) * knockback
  
  
  facing = -Sign(*defending\physics\v\x)
  If knockback > 10
    type = #KB_TUMBLE
    If degAngle > 60 And degAngle < 120 And *attacking\y < *defending\y
      anim = "hurtup"
      facing = 0
    ElseIf degAngle > 240 And degAngle < 300 And Not *defending\grounded
      anim = "hurtdown"
      facing = 0
    Else
      anim = "hurtheavy"
    EndIf   
  Else
    anim = "hurt"
  EndIf 
  If *defending\physics\v\y < 2 And *defending\physics\v\y > 0 And *defending\grounded And Not type = #KB_TUMBLE
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

;modifier la formule de vitesse (kb * 0.5 * sign(cos(angle)) + kb * 0.5 * cos(agnle))
;voir pour l'anim

Procedure shieldHit(*hitbox.Hitbox, *hurtbox.Hurtbox, *attacking.Fighter, *defending.Fighter)
  Define hitlag.b, type.b, hitstun.l, anim.s, facing.b, angle.d, direction.b
  If *attacking\facing = 1
    degAngle.l = *hitbox\angle
  Else
    degAngle.l = 180 - *hitbox\angle
  EndIf
  angle = Radian(degAngle)
  
  If *defending\state = #STATE_GUARDSTUN
    Debug "TRUE (shield)"
  EndIf 
 
  knockback = getShieldKnockback(*defending\damage, *hitbox\bkb, *hitbox\skb, *defending\character\weight)

  *defending\physics\v\x = knockback * Sign(Cos(angle))
  
  hitlag = getHitlag(*hitbox\damage)
  *defending\paused = hitlag
  *attacking\paused = hitlag
  hitstun = getShieldStun(knockback)
  ;setAnimation(*defending, anim, 0, facing)
  setState(*defending, #STATE_GUARDSTUN, hitstun)
EndProcedure

Procedure hit(*hitbox.Hitbox, *hurtbox.Hurtbox, *attacking.Fighter, *defending.Fighter, type.b)
  Define angle.d, knockback.d
  
  ForEach *attacking\fightersHit()
    If *attacking\fightersHit()\fighter = *defending And *attacking\fightersHit()\hitID = *hitbox\hit
      ProcedureReturn 0
    EndIf 
  Next
  
  AddElement(*attacking\fightersHit())  
  *attacking\fightersHit()\hitID = *hitbox\hit
  *attacking\fightersHit()\fighter = *defending
  
  Select type
    Case #HIT_CUSTOM
      ;custom state handling
    Case #HIT_SHIELD
      shieldHit(*hitbox, *hurtbox, *attacking, *defending)
    Default
      startKnockback(*hitbox, *hurtbox, *attacking, *defending)
    EndSelect
EndProcedure

Procedure manageHitboxes(*game.Game)
  Define *attacking.Fighter, *defending.Fighter, *hitbox.Hitbox, *hurtbox.Hurtbox, *successfulHitbox.Hitbox, shieldHit.b, *attackingFrame.Frame, *defendingFrame.Frame
  ForEach *game\fighters()
    *attacking = @*game\fighters()
    *attackingFrame = getCurrentFrame(*attacking\currentAnimation)
    ForEach  *game\fighters()
      *defending = @*game\fighters()
      *defendingFrame = getCurrentFrame(*defending\currentAnimation)
      If *defending = *attacking
        Continue
      EndIf 
      *successfulHitbox = 0
      shieldHit = 0
      ForEach *attackingFrame\hitboxes()
        *hitbox = @*attackingFrame\hitboxes()
        
        ;checking shield
        If *defending\state = #STATE_GUARD Or *defending\state = #STATE_GUARDSTUN
          If testRectCircleCollision(getRealCboxX(*hitbox, *attacking\facing) + *attacking\x, *attacking\y + *hitbox\y - *hitbox\y2, *hitbox\x2, *hitbox\y2, getActualShieldX(*defending), getActualShieldY(*defending), *defending\shieldSize * *defending\character\shieldInfo\size) 
            shieldHit = 1
            If Not *successfulHitbox Or *hitbox\priority > *successfulHitbox\priority
              *successfulHitbox = *hitbox
            EndIf
          EndIf
        EndIf
        
        ;checking hurtboxes 
        If Not shieldHit
          ForEach *defendingFrame\hurtboxes()
            *hurtbox = @*defendingFrame\hurtboxes()
            If testRectCollision(getRealCboxX(*hitbox, *attacking\facing) + *attacking\x, *attacking\y + *hitbox\y, *hitbox\x2, *hitbox\y2,
                                 getRealCboxX(*hurtbox, *defending\facing) + *defending\x, *defending\y + *hurtbox\y, *hurtbox\x2, *hurtbox\y2) 
              
              If Not *successfulHitbox Or *hitbox\priority > *successfulHitbox\priority
                *successfulHitbox = *hitbox
              EndIf 
            EndIf 
          Next
        EndIf 
        
      Next
      If *successfulHitbox 
        hit(*successfulHitbox, *hurtbox, *attacking, *defending, shieldHit)
      EndIf 
    Next
    ChangeCurrentElement(*game\fighters(), *attacking)  
  Next  
EndProcedure

;- Game methods ---



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
; CursorPosition = 707
; FirstLine = 676
; Folding = --v------
; EnableXP
; SubSystem = OpenGL
; EnableUnicode