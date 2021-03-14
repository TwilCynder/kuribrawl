;-structures

Structure Animation
    *model.AnimationModel
    currentFrame.b ;index of the current frame
    timeLeft.d
    speed.d ;current speed
    frameMultiplier.b ;default duration of all frames (depends on speed)
    carry.f
    currentCarry.f
    facing.b
    finished.b ;indicates that this animation has reached its end. It has already looped back to the first frame.
    onFrameChanged.f_callback ;TODO remplacer par un vrai système d'event
    endCallback.f_callback
EndStructure

Structure Sprite
    offensive.b ;indicates that hitbox collision tests should be made
    *fighter
    animation.Animation
    pos.Vector
EndStructure

Structure Camera Extends Vector
EndStructure

;- Rendering objects
CreateImage(0, #SCREEN_W, #SCREEN_H, 32, #PB_Image_Transparent)

Declare applyFrameMovement(*fighter, *frame.Frame)

Procedure resetAnimation(*animation.Animation) ;completely resets an Animation (to be used right before setting+initializing it again)
    *animation\currentFrame = 0
    *animation\currentCarry = 0
EndProcedure

Procedure currentAnimLength(*animation.Animation, speed.d = 0.0) ;returns the length of an animation if played with the specified speed, or its current speed, of its base speed.
    If (Not speed)
        If *animation\speed
            speed = *animation\speed
        Else
            speed = *animation\model\baseSpeed
        EndIf
    EndIf

    If speed > 1 Or speed = -1
        ProcedureReturn speed
    Else
        ProcedureReturn framesCount(*animation) * Int(1 / speed)
    EndIf
EndProcedure

Procedure getCurrentFrame(*animation.Animation)
    ProcedureReturn *animation\model\frames(*animation\currentFrame)
EndProcedure

;TODO rendre les speed < 1 plus précises
Procedure setAnimationSpeed(*anim.Animation, speed = 0)
    Define frameCount.b, ratio.d, cutFrames.b
    If speed = 0 ;if no speed is specified
        *anim\speed = *anim\model\baseSpeed
    Else
        *anim\speed = speed
    EndIf

    If *anim\speed = 0
        Debug "Can't set animation speed to 0 (neither the specified speed or the base speed were non-zero, there is a problem with the base speed)"
        *anim\speed = 1
    EndIf

    If *anim\speed > 1 ;the speed must be used as a total duration of the animation
        frameCount = framesCount(*anim\model)
        ratio = *anim\speed / frameCount
        *anim\frameMultiplier = Round(ratio, #PB_Round_Up)
        If frameCount > 1 And Not (*anim\frameMultiplier + 0.0 = ratio)
            cutFrames = (*anim\frameMultiplier * frameCount) - *anim\speed
            *anim\carry = (1 / (frameCount - 1) * cutFrames)
        EndIf
    Else
        *anim\frameMultiplier = 1 / *anim\speed
    EndIf
EndProcedure

Procedure initAnimation(*anim.Animation, *model.AnimationModel)
    *anim\currentFrame = 0
    *anim\carry = 0
    *anim\facing = 0
    *anim\frameMultiplier = 1
    *anim\onFrameChanged = 0
    *anim\speed = 1
    *anim\finished = 0

    *anim\model = *model
    *anim\endCallback = *model\endCallback
    *anim\onFrameChanged = *model\frameCallback
EndProcedure

Procedure startAnimation(*anim.Animation) ;to be called when an animation start
    *frame.Frame = getCurrentFrame(*anim)
    If *frame\duration
        *anim\timeLeft = *frame\duration
    ElseIf Not *anim\speed = 1 And Not *anim\speed = -1
        *anim\timeLeft = *anim\frameMultiplier
    EndIf
EndProcedure

Procedure setAnimationModel(*animation.Animation, *model.AnimationModel, speed.d = 0)
    initAnimation(*animation, *model)
    setAnimationSpeed(*animation, speed)
    startAnimation(*animation)
EndProcedure

;TODO supporter l'affichage des décimales
Procedure DrawDamageText(*font.Font, damage.l, x.l, y.l)
    If Not IsSprite(*font\fontImage)
        error("The current font image is not an initialized sprite")
    EndIf
    Define digit.b, started
    Define w = *font\fontDimensions\x
    digit = damage / 100
    If digit
        ClipSprite(*font\fontImage, digit * w, 0, w, *font\fontDimensions\y)
        DisplaySprite(*font\fontImage, x, y)
        x + w
        started = 1
    EndIf
    damage = damage - (digit * 100)
    digit = damage / 10
    If digit Or started
        ClipSprite(*font\fontImage, digit * w, 0, w, *font\fontDimensions\y)
        DisplaySprite(*font\fontImage, x, y)
        x + w
    EndIf
    digit = damage - digit * 10
    ClipSprite(*font\fontImage, digit * w, 0, w, *font\fontDimensions\y)
    DisplaySprite(*font\fontImage, x, y)
    ProcedureReturn x + w
EndProcedure

Procedure drawAnimationFrame(*frame.Frame, spriteSheet.l, x.l, y.l, facing = 1)
    Define dx.l
    With *frame
        If facing = 1
            dx = x -\origin\x
        Else
            dx = x + \origin\x -\display\w
        EndIf
        ClipSprite(spriteSheet,\display\x,\display\y,\display\w,\display\h)
        DisplayTransparentSprite(spriteSheet, dx, y -\origin\y)
    EndWith
EndProcedure

; TODO avancer les anims des stages (bg/plat)
Procedure nextFrame(*animation.Animation, *owner = 0)
    Define *frame.Frame, res.i
    *animation\currentFrame + 1
    *animation\finished = 0

    If *animation\currentFrame >= *animation\model\frameNb
        *animation\finished = 1
        If *animation\endCallback
            If *animation\endCallback = 1
                ProcedureReturn
            EndIf
            res = *animation\endCallback(*owner, 0)
            If res > 0
                ProcedureReturn res
            EndIf
        EndIf
        resetAnimation(*animation)
    EndIf

    *frame = getCurrentFrame(*animation)
    If *frame\duration
        *animation\timeLeft = *frame\duration
    ElseIf Not *animation\speed = 1 And Not *animation\speed = -1
        *animation\timeLeft = *animation\frameMultiplier
        If *animation\currentCarry >= 1
            *animation\currentCarry - 1
            *animation\timeLeft - 1
        EndIf
    EndIf

    If *animation\onFrameChanged
        *animation\onFrameChanged(*owner, *frame)
    EndIf

    If *owner And *frame\speedMode & 1 And Not (*frame\speedMode & %100)
        applyFrameMovement(*owner, *frame)
    EndIf
EndProcedure

Procedure advanceAnimation(*animation.Animation, *owner = 0)
    If Not *animation\speed = -1 And *animation\model
        *animation\timeLeft - 1
        If *animation\timeLeft <= 0
            *animation\currentCarry + *animation\carry
            nextFrame(*animation, *owner)
            ProcedureReturn 1
        EndIf
    EndIf
EndProcedure

Procedure renderAnimation(*animation.Animation, *camera.Camera, x.l, y.l, useLeftSpritesheet.b = 0)
    *frame.Frame = getCurrentFrame(*animation)

    If useLeftSpritesheet And *animation\model\spriteSheetL
        drawAnimationFrame(*frame, *animation\model\spriteSheetL, x, y, facing)
    Else
        drawAnimationFrame(*frame, *animation\model\spriteSheet, x, y, facing)
    EndIf

    ProcedureReturn *frame
EndProcedure

Procedure skipToFrame(*animation.Animation, frame.b) ; not compatible with animations with anim modifiers. or maybe it is. idk
    *animation\currentFrame = frame
EndProcedure

Procedure setSpriteAnimation(*sprite.Sprite, *model.AnimationModel, speed.d = 0)
    If Not *model
        ProcedureReturn 1
    EndIf
    setAnimationModel(*sprite\animation, *model, speed)
EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 205
; FirstLine = 174
; Folding = ---
; EnableXP