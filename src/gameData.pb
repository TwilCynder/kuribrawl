stateDefaultAnimation(#STATE_WALK) = "walk"
stateDefaultAnimation(#STATE_IDLE) = "idle"
stateDefaultAnimation(#STATE_DASH) = "dash"
stateDefaultAnimation(#STATE_JUMPSQUAT) = "jumpsquat"
stateDefaultAnimation(#STATE_DASH_STOP) = "dashStop"
stateDefaultAnimation(#STATE_DASH_START) = "dashStart"
stateDefaultAnimation(#STATE_DASH_TURN) = "dashTurn"

;- ========VARIABLES=====================

kuribrawl\variables\walkingJumpBoost = 3.0
kuribrawl\variables\backwardJumpBoost = 3.0
kuribrawl\variables\doubleJumpBackwardSpeed = 3.0

;- ========CHARACTER SPECIFIC============


*c1.Champion = newCharacter("Acid Rainbows")
*testAnim.Animation = newAnimation(*c1, "idle", "res/idle_strip4.png", 0.05)
addFrame(*testAnim, 0, 0, 34, 72, 17, 72)
addFrame(*testAnim, 34, 0, 34, 72, 17, 72)
addFrame(*testAnim, 68, 0, 34, 72, 17, 72)
addFrame(*testAnim, 102, 0, 34, 72, 17, 72)
resetAnimation(*testAnim)

*testAnim.Animation = newAnimation(*c1, "walk", "res/walk_strip6.png", 0.15)
addFrame(*testAnim, 0, 0, 30, 70, 15, 70)
addFrame(*testAnim, 30, 0, 30, 70, 15, 70)
addFrame(*testAnim, 60, 0, 30, 70, 15, 70)
addFrame(*testAnim, 90, 0, 30, 70, 15, 70)
addFrame(*testAnim, 120, 0, 30, 70, 15, 70)
addFrame(*testAnim, 150, 0, 30, 70, 15, 70)
resetAnimation(*testAnim)

*testAnim = newAnimation(*c1, "dash", "res/dash_strip4.png", 0.18)
addFrame(*testAnim, 0, 0, 62, 62, 31, 62)
addFrame(*testAnim, 62, 0, 62, 62, 31, 62)
addFrame(*testAnim, 124, 0, 62, 62, 31, 62)
addFrame(*testAnim, 186, 0, 62, 62, 31, 62)
resetAnimation(*testAnim)

*testAnim = newAnimation(*c1, "jumpsquat", "res/jumpstart.png", -1)
addFrame(*testAnim, 0, 0, 40, 56, 20, 56)

*testAnim = newAnimation(*c1, "dashStop", "res/dashstop_strip2.png")
addFrame(*testAnim, 0, 0, 50, 68, 25, 68)
addFrame(*testAnim, 50, 0, 50, 68, 25, 68)

*testAnim = newAnimation(*c1, "dashStart", "res/dashstart_strip2.png", 0.25)
addFrame(*testAnim, 0, 0, 44, 66, 22, 66)
addFrame(*testAnim, 44, 0, 44, 66, 22, 66)

*testAnim = newAnimation(*c1, "dashTurn", "res/dashturn_strip3.png", 0.2)
addFrame(*testAnim, 0, 0, 44, 64, 22, 64)
addFrame(*testAnim, 44, 0, 44, 64, 22, 64)
addFrame(*testAnim, 88, 0, 44, 64, 22, 64)


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

; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 52
; FirstLine = 25
; EnableXP