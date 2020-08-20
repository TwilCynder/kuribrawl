stateDefaultAnimation(#STATE_WALK) = "walk"
;stateDefaultAnimation(#STATE_IDLE) = "idle"
stateDefaultAnimation(#STATE_DASH) = "dash"
stateDefaultAnimation(#STATE_JUMPSQUAT) = "jumpsquat"
stateDefaultAnimation(#STATE_DASH_STOP) = "dashStop"
stateDefaultAnimation(#STATE_DASH_START) = "dashStart"
stateDefaultAnimation(#STATE_DASH_TURN) = "dashTurn"
stateDefaultAnimation(#STATE_LANDING) = "land"
stateDefaultAnimation(#STATE_LANDING_LAG) = "land"

commandDefaultAnimation(#COMMAND_NAir) = "nair"
commandDefaultAnimation(#COMMAND_FAir) = "fair"
commandDefaultAnimation(#COMMAND_UAir) = "uair"
commandDefaultAnimation(#COMMAND_BAir) = "nair"
commandDefaultAnimation(#COMMAND_DAir) = "dair"
commandDefaultAnimation(#COMMAND_UTilt) = "utilt"
commandDefaultAnimation(#COMMAND_Jab) = "jab"

;- ========VARIABLES=====================

kuribrawl\variables\walkingJumpBoost = 3.0
kuribrawl\variables\backwardJumpBoost = 3.0
kuribrawl\variables\doubleJumpBackwardSpeed = 3.0
kuribrawl\variables\cameraMaxSpeed = 10

;- ========STAGE SPECIFIC================
*s1.StageModel = newStage("Snowdin", #SCREEN_W * 1.5, #SCREEN_H)
setStageCameraZone(*s1, #SCREEN_W * 1.2, #SCREEN_H)
;addPlatform(*s1, -200, 400, 500, "platform")
;addPlatform(*s1, 100, 200, 500, "platform")
addCenteredPlatform(*s1, 200, 850, "platform")

;- ========CHARACTER SPECIFIC============

*c1.Champion = newCharacter("Acid")
*c1\displayName = "Acid Rainbows"

*c1\moves(#COMMAND_NAir)\landLag = 4
*c1\moves(#COMMAND_UAir)\landLag = 6
*c1\moves(#COMMAND_DAir)\landLag = 8
*c1\moves(#COMMAND_BAir)\landLag = 5
*c1\moves(#COMMAND_FAir)\landLag = 8

makeMultiMove(*c1\moves(#COMMAND_Jab), 3)
*c1\moves(#COMMAND_Jab)\multiMove\partEndFrames(0) = 3
*c1\moves(#COMMAND_Jab)\multiMove\partStartFrames(0) = 5
*c1\moves(#COMMAND_Jab)\multiMove\partEndFrames(1) = 9
*c1\moves(#COMMAND_Jab)\multiMove\partStartFrames(1) = 12


*c1\dashSpeed = 8.0
*c1\initialDashSpeed = 10.0
*c1\dashTurnAccel = 1.5
*c1\walkSpeed = 4.5
*c1\maxAirSpeed = 5.0
*c1\airAcceleration = 0.3
*c1\airFriction = 0.05
*c1\traction = 0.7
*c1\jumpSpeed = 13.0
*c1\jumpsquatDuration = 5
*c1\shortHopSpeed = 7.5
*c1\doubleJumpSpeed = 12.0
*c1\fastFallSpeed = 16.0
*c1\maxFallSpeed = 11.0
*c1\dashStopDuration = 0
*c1\dashStartDuration = 0
*c1\dashTurnDuration = 0
*c1\landingDuration = 0

; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 23
; EnableXP