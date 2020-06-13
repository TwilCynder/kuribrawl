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

*testAnim.Animation = newAnimation(*c1, "dash", "res/dash_strip4.png", 0.18)
addFrame(*testAnim, 0, 0, 62, 62, 31, 62)
addFrame(*testAnim, 62, 0, 62, 62, 31, 62)
addFrame(*testAnim, 124, 0, 62, 62, 31, 62)
addFrame(*testAnim, 186, 0, 62, 62, 31, 62)
resetAnimation(*testAnim)

*testAnim.Animation = newAnimation(*c1, "jumpsquat", "res/jumpstart.png", -1)
addFrame(*testAnim, 0, 0, 40, 56, 20, 56)

*c1\dashSpeed = 8.0
*c1\initialDashSpeed = 110.0
*c1\walkSpeed = 4.5
*c1\maxAirSpeed = 40.0
*c1\airAcceleration = 0.3
*c1\traction = 0.7
*c1\jumpSpeed = 13.0
*c1\jumpsquatDuration = 5

; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 31
; EnableXP