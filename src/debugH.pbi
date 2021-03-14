Define stateGadget.q, previousStateGadget.q, previousStateTimeGadget.q, inputConsole.q

Procedure InputLogGadget(x.l, y.l, w.l, h.l)
  Shared inputConsole
  inputConsole = ListIconGadget(-1, x, y, w, h, "Port", 20)
  AddGadgetColumn(inputConsole, 1, "Input", w - 100)
  AddGadgetColumn(inputConsole, 2, "Element", 30)
  AddGadgetColumn(inputConsole, 3, "Frame", 40)
  ProcedureReturn inputConsole
EndProcedure

Procedure StatesGadgets(x.l, y.l, w.l)
  Shared stateGadget, previousStateGadget, previousStateTimeGadget
  TextGadget(-1, x, y, w, 20, "Previous State")
  previousStateGadget = StringGadget(-1, x, y + 25, w - 35, 20, "")
  previousStateTimeGadget = StringGadget(-1, x + w - 30, y + 25, 30, 20, "")
  TextGadget(-1, x, y + 50, w, 20, "Current State")
  stateGadget = StringGadget(-1, x, y + 75, w, 20, "")
EndProcedure

Declare logInput(port.b, input.b, element.b, stick.b, frame.l = -1)
Declare logState(state.b, facing.b = 0, previousTime.u = 0)
; IDE Options = PureBasic 5.72 (Windows - x64)
; Folding = -
; EnableXP