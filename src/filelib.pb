NewMap loadedSprites.l()

Procedure checkSignature()
  If Not ReadInteger(0) = $54545454
    Debug "error"
    End
  EndIf
EndProcedure

Procedure readVersion()
  Define maj.a, min.a, rev.a
  maj = ReadAsciiCharacter(0)
  min = ReadAsciiCharacter(0)
  rev = ReadAsciiCharacter(0)
  Debug maj
  Debug min
  Debug rev
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
  *buffer = ReAllocateMemory(*buffer, size)
  ReadData(0, *buffer, size)
  ProcedureReturn *buffer
EndProcedure

Procedure checkInterfile()
  If Not ReadAsciiCharacter(0) = $54
    Debug "error"
    End
  EndIf
EndProcedure

Procedure LoadSprite_(*buffer, tag.s)
  Shared loadedSprites()
  loadedSprites(tag) = CatchSprite(#PB_Any, *buffer, #PB_Sprite_AlphaBlending)
EndProcedure

Procedure loadGameData()
  Shared loadedSprites()
  If #DEBUG
    OpenFile(0, "res/data.twl")
  Else 
    OpenFile(0, "data.twl")
  EndIf 
  checkSignature()
  readVersion()

  Define type.b, tag.s, size.l, *buffer

  *buffer = 0
  Repeat
    type = readFileType()
    tag = readFileTag()
    size = readFileLength()
    *buffer = readFileData(size, *buffer)
    LoadSprite_(*buffer, tag)
    checkInterfile()
  Until Eof(0)
  
  CloseFile(0)
 EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 55
; FirstLine = 19
; Folding = --
; EnableXP