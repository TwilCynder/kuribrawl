Define stateGadget.q, previousStateGadget.q, inputConsole.q

XIncludeFile "debugData.pbi"

Procedure InputLogGadget(x.l, y.l, w.l, h.l)
  Shared inputConsole
  inputConsole = ListIconGadget(-1, x, y, w, h, "Port", 20)
  AddGadgetColumn(inputConsole, 1, "Input", w - 60)
  AddGadgetColumn(inputConsole, 2, "Element", 30)
  ProcedureReturn inputConsole
EndProcedure

Procedure logInput(port.b, input.b, element.b, stick.b)
  Shared inputConsole, InputNames()
  AddGadgetItem(inputConsole, 0, Str(port) + Chr(10) + InputNames(input) + Chr(10) + Str(element * -((stick * 2) -1 )))
EndProcedure

Procedure StatesGadgets(x.l, y.l, w.l)
  Shared stateGadget, previousStateGadget 
  TextGadget(-1, x, y, w, 20, "Previous State")
  previousStateGadget = StringGadget(-1, x, y + 25, w, 20, "")
  TextGadget(-1, x, y + 50, w, 20, "Current State")
  stateGadget = StringGadget(-1, x, y + 75, w, 20, "")
EndProcedure

Procedure logState(state.b, facing.b = 0)
  Shared stateGadget, previousStateGadget, StateNames()
  SetGadgetText(previousStateGadget, GetGadgetText(stateGadget))
  SetGadgetText(stateGadget, StateNames(state))
  If facing = 1
    SetGadgetColor(stateGadget, #PB_Gadget_BackColor,  #Cyan)
  ElseIf facing = -1
    SetGadgetColor(stateGadget, #PB_Gadget_BackColor,  #Red)
  EndIf 
EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 30
; Folding = -
; EnableXP