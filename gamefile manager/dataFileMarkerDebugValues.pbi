Structure DebugValues
    fileTypeNames.s[#FILETYPES]
    hurtboxValues.s[5]
    hitboxValues.s[11]
    hitboxTypes.s[3]
EndStructure

Procedure.i initDebugValues()
    *vals.DebugValues = AllocateMemory(SizeOf(DebugValues))
    
    *vals\fileTypeNames[0] = "Animation"
    *vals\fileTypeNames[1] = "Champion"
    *vals\fileTypeNames[2] = "Left Animation"
    *vals\fileTypeNames[3] = "Simple image"
    *vals\fileTypeNames[4] = "Stage"
    
    *vals\hitboxValues[0] = "x"
    *vals\hitboxValues[1] = "y"
    *vals\hitboxValues[2] = "w"
    *vals\hitboxValues[3] = "h"
    *vals\hitboxValues[4] = "type"
    *vals\hitboxValues[5] = "damage"
    *vals\hitboxValues[6] = "angle"
    *vals\hitboxValues[7] = "base knockback"
    *vals\hitboxValues[8] = "scaling knockback"
    *vals\hitboxValues[9] = "hitID"
    *vals\hitboxValues[10] = "priority"
    
    *vals\hurtboxValues[0] = "x"
    *vals\hurtboxValues[1] = "y"
    *vals\hurtboxValues[2] = "w"
    *vals\hurtboxValues[3] = "h"
    *vals\hurtboxValues[4] = "type"
    
    *vals\hitboxTypes[0] = "damaging (normal)"
    *vals\hitboxTypes[1] = "grab"
    *vals\hitboxTypes[2] = "special"
    
    ProcedureReturn *vals
EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 4
; Folding = -
; EnableXP