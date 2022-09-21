Procedure createGImage(w.l, h.l)
  size.l = SizeOf(GImage) + (w * h * 4)
  *img.GImage = AllocateMemory(size)
  *img\size = w * h * 4
  *img\w = w
  *img\h = h
EndProcedure

Procedure resetGImage(*img.GImage)
  FillMemory(*img imgBuffer, *img\size)
EndProcedure

Procedure fillGImageColor(*img.GImage, color.l)
  FillMemory(*img imgBuffer, *img\size, color, #PB_Integer)
EndProcedure

Procedure drawGImage(*img.GImage, x.l, y.l, *output.Long)
  Define *writeAdress.Long
  For cy = 0 To *img\h - 1
    *writeAdress = *output + (cy)
    For cx = 0 To *img\w - 1
      
    Next
  Next
EndProcedure

Structure Pixel
  color.l
EndStructure

Procedure directDraw_Circle(x, y, r, color, *output)
  Define area.RECT
  area\left = x - r
  If area\left < 0
    area\left = 0
  EndIf 
  area\top = y - r
  If area\top < 0
    area\top = 0
  EndIf 
  area\right = x + r
  If area\right > OutputWidth()
    area\right = OutputWidth()
  EndIf 
  area\bottom = y + r
  If area\bottom > OutputHeight()
    area\bottom = OutputHeight()
  EndIf 
  Define *lineStart.Pixel = *output + (DrawingBufferPitch() * area\top) + (area\left * 4)
  Define pitch = DrawingBufferPitch()
  Define cx.l, cy.l
  cy = area\top
  
  While cy < area\bottom
    cx = area\left
    While cx < area\right
      PokeL(*lineStart + (cx * 4), color)
      cx + 1
    Wend
    *lineStart + DrawingBufferPitch()
    cy + 1
  Wend
EndProcedure


Structure GImage
  w.l
  h.l
  size.l
EndStructure

Macro imgBuffer
  + SizeOf(GImage)
EndMacro


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
; CursorPosition = 134
; FirstLine = 83
; Folding = ---
; EnableXP