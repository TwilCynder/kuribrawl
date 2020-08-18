Enumeration
  #DIRECTION_RIGHT
  #DIRECTION_UP
  #DIRECTION_LEFT
  #DIRECTION_DOWN
  #DIRECTION_NONE
EndEnumeration

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

;- Queue
Structure QueueElem
  *next.QueueElem
  value.l
EndStructure

Structure Queue
  *tail.QueueElem
  *sentinel.QueueElem
EndStructure

Procedure newQueue()
  *q.Queue = AllocateMemory(SizeOf(Queue))
  *q\sentinel = AllocateMemory(SizeOf(Queue))
  *q\sentinel\value = 0
  *q\sentinel\next = *q\sentinel
  *q\tail = *q\sentinel
  ProcedureReturn *q
EndProcedure

Macro QHead
  sentinel\next
EndMacro

Macro QSize
  sentinel\value
EndMacro

Procedure QPush(*q.Queue, value.b)
  *elem.QueueElem = AllocateMemory(SizeOf(QueueElem))
  *elem\value = value
  
  *q\tail\next = *elem
  *q\tail = *elem
  *elem\next = *q\sentinel
  *q\QSize + 1
EndProcedure

Procedure.b QPop(*q.Queue)
  res.b = *q\QHead\value
  *oldHead = *q\Qhead
  *q\QHead = *q\QHead\next
  *q\QSize - 1
  If *q\QSize < 1
    *q\tail = *q\sentinel
  EndIf
  FreeMemory(*oldHead)
  ProcedureReturn res
EndProcedure

Prototype QCallback(value.l)
Procedure QIterate(*q.Queue, callback.QCallback)
  *elem.QueueElem = *q\sentinel
  While Not (*elem\next = *q\sentinel)
    *elem = *elem\next
    callback(*elem\value)
  Wend
EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 18
; Folding = --
; EnableXP