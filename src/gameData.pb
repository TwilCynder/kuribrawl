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
commandDefaultAnimation(#COMMAND_FAir) = "nair"
commandDefaultAnimation(#COMMAND_UAir) = "nair"
commandDefaultAnimation(#COMMAND_BAir) = "nair"
commandDefaultAnimation(#COMMAND_DAir) = "nair"

;- ========VARIABLES=====================

kuribrawl\variables\walkingJumpBoost = 3.0
kuribrawl\variables\backwardJumpBoost = 3.0
kuribrawl\variables\doubleJumpBackwardSpeed = 3.0

;- ========CHARACTER SPECIFIC============


*c1.Champion = newCharacter("Acid Rainbows")
*testAnim.Animation = newAnimation(*c1, "idle", "Acid/idle", 0.05)
addLeftSpritesheet(*testAnim, "Acid/idleL")
addFrame(*testAnim, 0, 0, 34, 72, 17, 72)
addFrame(*testAnim, 34, 0, 34, 72, 17, 72)
addFrame(*testAnim, 68, 0, 34, 72, 17, 72)
addFrame(*testAnim, 102, 0, 34, 72, 17, 72)
resetAnimation(*testAnim)

*testAnim.Animation = newAnimation(*c1, "walk", "Acid/walk", 0.15)
addLeftSpritesheet(*testAnim, "Acid/walkL")
addFrame(*testAnim, 0, 0, 30, 70, 15, 70)
addFrame(*testAnim, 30, 0, 30, 70, 15, 70)
addFrame(*testAnim, 60, 0, 30, 70, 15, 70)
addFrame(*testAnim, 90, 0, 30, 70, 15, 70)
addFrame(*testAnim, 120, 0, 30, 70, 15, 70)
addFrame(*testAnim, 150, 0, 30, 70, 15, 70)
resetAnimation(*testAnim)

*testAnim = newAnimation(*c1, "dash", "Acid/dash", 0.18)
addLeftSpritesheet(*testAnim, "Acid/dashL")
addFrame(*testAnim, 0, 0, 62, 62, 31, 62)
addFrame(*testAnim, 62, 0, 62, 62, 31, 62)
addFrame(*testAnim, 124, 0, 62, 62, 31, 62)
addFrame(*testAnim, 186, 0, 62, 62, 31, 62)
resetAnimation(*testAnim)

*testAnim = newAnimation(*c1, "jumpsquat", "Acid/jumpsquat", -1)
addLeftSpritesheet(*testAnim, "Acid/jumpsquatL")
addFrame(*testAnim, 0, 0, 40, 56, 20, 56)

*testAnim = newAnimation(*c1, "dashStop", "Acid/dashStop")
addLeftSpritesheet(*testAnim, "Acid/dashStopL")
addFrame(*testAnim, 0, 0, 50, 68, 25, 68)
addFrame(*testAnim, 50, 0, 50, 68, 25, 68)

*testAnim = newAnimation(*c1, "dashStart", "Acid/dashStart", 0.25)
addLeftSpritesheet(*testAnim, "Acid/dashStartL")
addFrame(*testAnim, 0, 0, 44, 66, 22, 66)
addFrame(*testAnim, 44, 0, 44, 66, 22, 66)

*testAnim = newAnimation(*c1, "dashTurn", "Acid/dashTurn", 13)
addLeftSpritesheet(*testAnim, "Acid/dashTurnL")
addFrame(*testAnim, 0, 0, 44, 64, 22, 64)
addFrame(*testAnim, 44, 0, 44, 64, 22, 64)
addFrame(*testAnim, 88, 0, 44, 64, 22, 64)

*testAnim = newAnimation(*c1, "jump", "Acid/jump", 0.5)
addLeftSpritesheet(*testAnim, "Acid/jumpL")
addFrame(*testAnim, 0, 0, 38, 86, 19, 86)
addFrame(*testAnim, 38, 0, 38, 86, 19, 86)
addFrame(*testAnim, 76, 0, 38, 86, 19, 86)
addFrame(*testAnim, 114, 0, 38, 86, 19, 86)
addFrame(*testAnim, 152, 0, 38, 86, 19, 86)

*testAnim = newAnimation(*c1, "airIdle", "Acid/airIdle", -1)
addLeftSpritesheet(*testAnim, "Acid/airIdleL")
addFrame(*testAnim, 0, 0, 38, 86, 19, 86)

*testAnim = newAnimation(*c1, "land", "Acid/land", 0.5)
addLeftSpritesheet(*testAnim, "Acid/landL")
addFrame(*testAnim, 0, 0, 44, 64, 22, 64)
addFrame(*testAnim, 44, 0, 44, 64, 22, 64)

*testAnim = newAnimation(*c1, "nair", "Acid/nair")
addLeftSpritesheet(*testAnim, "Acid/nairL")
addFrame(*testAnim, 0, 0, 68, 66, 34, 66, 4)
addFrame(*testAnim, 68, 0, 68, 66, 34, 66, 3)
addFrame(*testAnim, 136, 0, 68, 66, 34, 66, 2)
addFrame(*testAnim, 204, 0, 68, 66, 34, 66, 13)
addFrame(*testAnim, 272, 0, 68, 66, 34, 66, 3)
addFrame(*testAnim, 340, 0, 68, 66, 34, 66, 3)

*c1\moves(#COMMAND_NAir)\landLag = 20
*c1\moves(#COMMAND_UAir)\landLag = 20
*c1\moves(#COMMAND_DAir)\landLag = 20
*c1\moves(#COMMAND_BAir)\landLag = 20
*c1\moves(#COMMAND_FAir)\landLag = 20

initDefaultAnimationsConfig(*c1)

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
; CursorPosition = 102
; FirstLine = 79
; EnableXP