XIncludeFile "debugData.pbi"

Procedure logInput(port.b, input.b, element.b, stick.b, frame.l = -1)
  Shared inputConsole, InputNames()
  AddGadgetItem(inputConsole, 0, Str(port) + Chr(10) + InputNames(input) + Chr(10) + Str(element * -((stick * 2) -1 )) + Chr(10) + Str(frame))
EndProcedure

Procedure logState(state.b, facing.b = 0, previousTime.u = 0)
  Shared stateGadget, previousStateGadget, previousStateTimeGadget, StateNames()
  SetGadgetText(previousStateGadget, GetGadgetText(stateGadget))
  SetGadgetText(previousStateTimeGadget, Str(previousTime))
  SetGadgetText(stateGadget, StateNames(state))
  If facing = 1
    SetGadgetColor(stateGadget, #PB_Gadget_BackColor,  #Cyan)
  ElseIf facing = -1
    SetGadgetColor(stateGadget, #PB_Gadget_BackColor,  #Red)
  EndIf 
EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; Folding = -
; EnableXP