Enumeration 
  #TYPE_BYTE
  #TYPE_DOUBLE
  #TYPE_LONG
  #TYPE_FLOAT
EndEnumeration

NewMap loadedSprites.l()

Procedure getLoadedSprite(name.s)
  Shared loadedSprites()
  ProcedureReturn PeekL(FindMapElement(loadedSprites(), name))
EndProcedure

Procedure checkSignature()
  ProcedureReturn Bool(ReadInteger(0) = $54545454)
EndProcedure

Procedure readVersion()
  Define maj.a, min.a, rev.a
  maj = ReadAsciiCharacter(0)
  min = ReadAsciiCharacter(0)
  rev = ReadAsciiCharacter(0)
EndProcedure

Procedure readFileType()
  ProcedureReturn ReadAsciiCharacter(0)
EndProcedure

Procedure.s readFileTag()
  ProcedureReturn ReadString(0)
EndProcedure

Procedure readFileLength()
  ProcedureReturn ReadInteger(0)
EndProcedure

Procedure readFileData(size.l, *buffer)
  If size = 0
    ProcedureReturn 0
  EndIf 
  *buffer = ReAllocateMemory(*buffer, size)
  ReadData(0, *buffer, size)
  ProcedureReturn *buffer
EndProcedure

Procedure checkInterfile()
 ProcedureReturn Bool(ReadAsciiCharacter(0) = $54)
EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 7
; Folding = --
; EnableXP