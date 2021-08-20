Import "GMBCStringLib.lib"
    GMB_StringFieldUnicode.l(*string, field.l, separator.u, *startPos, *l)
    GMB_CountFieldsUnicode(*string, separator.u)
EndImport

Procedure.s GMB_StringField(line.s, field.l, separator.s)
    startPos.l
    length.l
    GMB_StringFieldUnicode(@line, field, Asc(separator), @startPos, @length)
    ProcedureReturn Mid(line, startPos, length)
EndProcedure

Macro GMB_StringField_(line, field, separator, output)
    GMB_StringFieldUnicode(@line, field, Asc(separator), @startPos, @length)
    output = Mid(line, startPos, length)
EndMacro

Procedure GMB_CountFields(line.s, separator.s)
    
    ProcedureReturn GMB_CountFieldsUnicode(@line, Asc(separator))
EndProcedure

Macro GMB_CountFields_(line, separator)
    GMB_CountFieldsUnicode(@line, Asc(separator))
EndMacro
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 12
; Folding = -
; EnableXP