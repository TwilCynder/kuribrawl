#SCREEN_W = 960
#SCREEN_H = 540

Enumeration 
  #ACTION_WALK
  #ACTION_DASH
  #ACTION_JUMP
  #ACTION_ATTACK
  #ACTION_NORMAL_ATTACK
  #ACTION_SPECIAL_ATTACK
  #ACTION_SHIELD
  #ACTION_DODGE
  #ACTION_GRAB
  
  #ACTIONS_TOTAL
EndEnumeration  

Enumeration
  #CBOX_TYPE_HURT
  #CBOX_TYPE_HIT
EndEnumeration

Enumeration
  #CBOX_SHAPE_CIRCLE
  #CBOX_SHAPE_BUBBLE
  #CBOX_SHAPE_SQUARE
  #CBOX_SHAPE_RECT
EndEnumeration

Enumeration
  #STATE_IDLE
  ;IDLE_AIR = IDLE && !grounded
  #STATE_CROUCH
  #STATE_WALK
  #STATE_WALK_TURN
  #STATE_DASH_START
  #STATE_DASH
  #STATE_DASH_TURN
  #STATE_DASH_STOP
  #STATE_JUMPSQUAT
  #STATE_LANDING
  #STATE_ATTACK
  ;ATTACK_AIR = ATTACK && !grounded
  #STATE_TUMBLE
  #STATE_LANDING_LAG
  #STATES
EndEnumeration

Enumeration
  #JUMP_NORMAL
  #JUMP_WALKING
  #JUMP_BACKWARDS
EndEnumeration

Enumeration
  #YJUMP_NORMAL
  #YJUMP_SHORT
  #YJUMP_DOUBLE
EndEnumeration

Enumeration ;commands
  #COMMAND_Jab
  #COMMAND_FTilt
  #COMMAND_UTilt
  #COMMAND_DTilt
  #COMMAND_FSmash
  #COMMAND_USmash
  #COMMAND_DSmash
  #COMMAND_DashAttack
  #COMMAND_NAir
  #COMMAND_FAir
  #COMMAND_BAir
  #COMMAND_UAir
  #COMMAND_DAir
  #COMMAND_ZAir
  #COMMAND_NSpecial
  #COMMAND_SSpecial
  #COMMAND_USpecial
  #COMMAND_DSpecial ;haha flip kick go brr
  #Command_Walk
  #Command_Dash
  #Command_Jump
  #COMMAND_Grab
  #COMMAND_Shield
  #COMMAND_Roll
  #COMMAND_SpotDodge
  #COMMAND_AirDodge
  #COMMAND_DirectionalAirDodge
  #COMMANDS
EndEnumeration

#MAX_BUTTON_NB = 31
#MAX_AXIS_NB = 8
; IDE Options = PureBasic 5.72 (Windows - x64)
; EnableXP