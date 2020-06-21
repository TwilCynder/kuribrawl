Enumeration 
  #FILETYPE_ANIMATION
EndEnumeration

Structure loadedFile
  *buffer
  size.l
EndStructure

Structure toLoad
  tag.s
  type.b
EndStructure

Procedure readFileToMemory(filename$, *file.loadedFile)
  ReadFile(0, filename$)
  *file\size = Lof(0)
  *file\buffer = ReAllocateMemory(*file\buffer, *file\size)
  ReadData(0, *file\buffer, *file\size)
  CloseFile(0)
  ProcedureReturn *buffer
EndProcedure

Procedure writeSignature()
  WriteInteger(1, $54545454)
EndProcedure

Procedure writeVersion(maj.a, min.a, rev.a)
  WriteAsciiCharacter(1, maj)
  WriteAsciiCharacter(1, min)
  WriteAsciiCharacter(1, rev)
EndProcedure  

Procedure writeType(type.a)
  WriteAsciiCharacter(1, type)
EndProcedure

Procedure writeFileTag(tag.s)
  WriteString(1, tag, #PB_Ascii)
  WriteAsciiCharacter(1, 0)
EndProcedure

Procedure WriteFileLength(len.l)
  WriteInteger(1, len)
EndProcedure

Procedure writeMemoryToFile(file, *file.loadedFile)
  WriteData(1, *file\buffer, *file\size)
EndProcedure

Procedure writeInterfile()
  WriteAsciiCharacter(1, $54)
EndProcedure

Procedure addFile(*f.loadedFile, path.s, tag.s, type.b)
  readFileToMemory(path, *f)
  writeType(type)
  writeFileTag(tag)
  WriteFileLength(*f\size)
  writeMemoryToFile(1, *f)
  writeInterfile()
EndProcedure

file.loadedFile
file\buffer = #Null

NewMap fileList.toLoad()

CreateFile(1, "data.twl")
writeSignature()
writeVersion(1, 3, 2)

Define tag.s, path.s, type.b
ReadFile(2, "project_db.txt")
While Not Eof(2)
  path = ReadString(2)
  tag = ReadString(2)
  Select StringField(tag, 1, ":")
    Case "A"
      type = #FILETYPE_ANIMATION
    Default:Continue
  EndSelect
  tag = StringField(tag, 2, ":")
  AddMapElement(fileList(), path)

  fileList()\tag = tag
  fileList()\type = type
Wend
CloseFile(2)

ForEach fileList()
  addFile(@file, MapKey(fileList()), fileList()\tag, fileList()\type)
Next 

CloseFile(1)
FreeMemory(file\buffer)

InitSprite()

UsePNGImageDecoder()
OpenWindow(0, 0, 0, 400, 400, "Test", #PB_Window_ScreenCentered)
OpenWindowedScreen(WindowID(0), 0, 0, 400, 400)



; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 68
; FirstLine = 45
; Folding = --
; EnableXP
; Executable = datafileMaker.exe