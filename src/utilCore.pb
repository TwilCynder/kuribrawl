Enumeration
  #DIRECTION_RIGHT
  #DIRECTION_UP
  #DIRECTION_LEFT
  #DIRECTION_DOWN
  #DIRECTION_NONE
EndEnumeration

#RedAlpha = %11111111000000000000000011111111
#BlueAlpha = %11111111111111110000000000000000
#GreenAlpha = %11111111000000001111111100000000
#YellowAlpha = %11111111000000001111111111111111

Macro error(stuff)
  Debug stuff
  End
EndMacro

Macro DebugHex(stuff)
  Debug Hex(stuff)
EndMacro

Macro getField(pointer, Structure, field, type)
  Peek#type(pointer + OffsetOf(Structure\field))
EndMacro
  
Macro setField(pointer, Structure, field, type, value)
  Poke#type(pointer + OffsetOf(Structure\field), value)
EndMacro

Macro addElementVal(l, value)
  AddElement(l) : l = value
EndMacro

;- Basic data structures

Structure Vector
  x.l
  y.l
EndStructure

Structure VectorDouble
  x.d
  y.d
EndStructure

Structure Rect_ Extends Vector
  w.l
  h.l
EndStructure

Macro BGR(r, g, b)
  RGB(b, g, r)
EndMacro

Macro BGRA(r, g, b, a)
  RGBA(b, g, r, a)
EndMacro
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 20
; Folding = --
; EnableXP