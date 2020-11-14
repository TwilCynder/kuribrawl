Structure Vector
  x.l
  y.l
EndStructure

Procedure testRectCircleCollision(rx.l, ry.l, rw.l, rh.l, cx.l, cy.l, cr.l)
  Define testPos.Vector
  Define testDist.Vector
  Define distance.l
  testPos\x = cx
  testPos\y = cy
  If cx < rx 
    testPos\x = rx ;left
  ElseIf cx > rx + rw
    testPos\x = rx + rw ;right
  EndIf 
  
  If cy < ry 
    testPos\y = ry ;above
  ElseIf cy > ry + rh
    testPos\y = ry + rh ;below
  EndIf 
  
  testDist\x = cx - testPos\x
  testDist\y = cy - testPos\y
  distance = Int(Sqr((testDist\x * testDist\x) + (testDist\y * testDist\y)))
  
  If distance <= cr
    ProcedureReturn 1
  EndIf
  
  ProcedureReturn 0
EndProcedure

Define color.l = #Red
Define rx, ry, rw, rh, cx, xy, cr
rx = 605
ry = 362
rw = 40 
rh = 14
cr = 40
cx = 594
cy = 330

InitSprite()

OpenWindow(0, 0, 0, 1000, 1000, "test", #PB_Window_ScreenCentered | #PB_Window_SystemMenu)
OpenWindowedScreen(WindowID(0), 0, 0, 1000, 1000)

For i = -100 To 300 Step 1
  ;cy = i
    ClearScreen(0)
  StartDrawing(ScreenOutput())
  If testRectCircleCollision(rx, ry, rw, rh, cx, cy, cr)
    color = #Green
  Else
    color = #Red
  EndIf
  Box(rx, ry, rw, rh, color)
  Circle(cx, cy, cr, color)
  StopDrawing()
  FlipBuffers()
  Delay(16)
  If WindowEvent() = #PB_Event_CloseWindow
    End
  EndIf
Next


; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 49
; FirstLine = 17
; Folding = -
; EnableXP