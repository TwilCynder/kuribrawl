Name|Description|Infos
-|-|-
IDLE|Basic state.|1 if just jumped.
GUARD|Shield|*none*
GUARD_START|Shield start (the shield hasn't came out yet)|Duration 
GUARD_STOP|Shield endlag (the shield is already gone)|Duration
WALK|Walking|*none*
WALK_TURN|Transition from walk to walk in the other direction|
DASH|Set after inital dash is finished|*none*
DASH_START|Initial dash (set on dash input is read)
DASH_STOP|Set when the stick is released during a dash
JUMPSQUAT|Set on jump input, the jump occurs when this state ends|Jump type X (walking/normal, on 1 bit), Jump type Y (full/short, on 1 bit), Controller Element (type on 2 bits, id on 5 bits)
LANDING|Set when going from airborne to grounded|*none*
ATTACK|Attacking|Attack (command id, on 8 bits), ControllerElement (type on 2 bits, id on 5 bits)
HITSTUN|Stun after getting hit|Type (normal/tumble, on 1 bit), Duration
GUARDSTUN|Stun from getting hit during shield (can't put down shield)|Duration
TUMBLE|Set after hitstun for a *heavy* hit ends. Only allow attacks.|*none*
LANDING_LAG|Set when going from airborn to grounded while attacking|Duration
CROUCH|Crouching (set after crouch start is finished)|*none*
CROUCH_START|Set when idle + grounded + stick down|*none*
CROUCH_STOP|Set when crouch released|*none*
CUSTOM|Custom state, defined by a Lua object (TBA)|Adress of the Lua object
