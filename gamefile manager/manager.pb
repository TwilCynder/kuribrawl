#FILEMARKER_DESCRIPTORSTART = $53
#FILEMARKER_ANIMSPEED = 1
#FILEMARKER_FRAMEINFO = 2
#FILEMARKER_FRAMEDURATION = $20
#FILEMARKER_FRAMEORIGIN = $21
#FILEMARKER_HURTBOXINFO = 3
#FILEMARKER_HITBOXINFO = 4
#FILEMARKER_INTERFILE = $54

Enumeration 
  #FILETYPE_ANIMATION
  #FILETYPE_LEFTANIM
EndEnumeration

Structure loadedFile
  *buffer
  size.l
EndStructure

Structure toLoad
  tag.s
  type.b
  infos.s
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
  WriteAsciiCharacter(1, #FILEMARKER_INTERFILE)
EndProcedure

Procedure writeFileDescriptor(type.b, infos.s)
  Define line.s, value.s, i.b
  
  If infos = ""
    ProcedureReturn 0
  EndIf   
  Select type
    Case #FILETYPE_ANIMATION
      PrintN("- starting descriptor")
      WriteByte(1, #FILEMARKER_DESCRIPTORSTART)
      
      If Right(infos, 4) = ".dat"

        PrintN("- descriptor file : " + infos)
        ReadFile(2, infos)
        line = ReadString(2)
        WriteByte(1, Val(line))
        PrintN("- - frame number : " + line)
        While Not Eof(2)
          line = ReadString(2)
          Select Left(line, 1)
            Case "s"
              value = Mid(line, 2)
              PrintN("- - anim speed : " + value)
              WriteByte(1, #FILEMARKER_ANIMSPEED)
              WriteFloat(1, ValF(value))
            Case "f"
              value = Mid(StringField(line, 1, " "), 2)
              WriteByte(1, #FILEMARKER_FRAMEINFO)
              WriteByte(1, Val(value))
              PrintN("- - frame modified : " + value)
              i = 2
              value = StringField(line, i, " ")
              While value
                If Left(value, 1) = "d"
                  WriteByte(1, $20)
                  WriteUnicodeCharacter(1, Val(Mid(value, 2)))
                  PrintN("- - - modified duration : " + Mid(value, 2))
                EndIf 
                i + 1
                value = StringField(line, i, " ")
              Wend 
            Case "c"
              value = Mid(StringField(line, 1, " "), 2)
              WriteByte(1, #FILEMARKER_HURTBOXINFO)
              WriteByte(1, Val(value))
              PrintN("- - adding hurtbox to frame : " + value)
              value = StringField(line, 2, " ")
              If value
                WriteWord(1, Val(value))
                PrintN("- - - value : " + value)
                For i = 3 To 5
                  value = StringField(line, i, " ")
                  PrintN("- - - value : " + value)
                  WriteWord(1, Val(value))
                Next 
              Else
                WriteWord(1, $5454)
                PrintN("- - - no value specified")
              EndIf 
            Case "h"
              value = Mid(StringField(line, 1, " "), 2)
              WriteByte(1, #FILEMARKER_HITBOXINFO)
              WriteByte(1, Val(value))
              PrintN("- - adding hitbox to frame : " + value)
              For i = 2 To 5
                PrintN("- - - value : " + value)
                value = StringField(line, i, " ")
                WriteWord(1, Val(value))
              Next 
              PrintN("- - - damages : " + StringField(line, 6, " "))
              WriteFloat(1, Val(StringField(line, 6, " ")))
          EndSelect
        Wend 
      Else
        value = StringField(infos, 1, " ")
        PrintN("- frame number : " + value)
        WriteByte(1, Val(value))
        value = StringField(infos, 2, " ")
        If value
          PrintN("- speed : " + value)
          WriteByte(1, #FILEMARKER_ANIMSPEED)
          WriteFloat(1, ValF(value))
        EndIf 
      EndIf
    Case #FILETYPE_LEFTANIM
      PrintN(">> left anim")
  EndSelect
EndProcedure


Procedure addFile(*f.loadedFile, path.s, tag.s, type.b, info.s)
  PrintN("file path : " + path)
  PrintN("tag : " + tag)
  readFileToMemory(path, *f)
  writeType(type)
  writeFileTag(tag)
  WriteFileLength(*f\size)
  writeMemoryToFile(1, *f)
  writeFileDescriptor(type, info)
  writeInterfile()
EndProcedure

file.loadedFile
file\buffer = #Null

NewMap fileList.toLoad()

OpenConsole()

CreateFile(1, "data.twl")
writeSignature()
writeVersion(0, 0, 1)

Define tag.s, path.s, type.b, infos.s
ReadFile(2, "project_db.txt")
While Not Eof(2)
  path = ReadString(2)
  tag = ReadString(2)
  
  infos = Mid(tag, FindString(tag, " ") + 1)
  tag = StringField(tag, 1, " ")
  ;parse tag  
  Select StringField(tag, 1, ":")
    Case "A"
      type = #FILETYPE_ANIMATION
    Case "AL"
      type = #FILETYPE_LEFTANIM
    Default:Continue
  EndSelect
  tag = StringField(tag, 2, ":")
  
  AddMapElement(fileList(), path)

  fileList()\tag = tag
  fileList()\type = type
  fileList()\infos = infos
Wend
CloseFile(2)

ForEach fileList()
  addFile(@file, MapKey(fileList()), fileList()\tag, fileList()\type, fileList()\infos)
Next 

CloseFile(1)
FreeMemory(file\buffer)

Input()




; IDE Options = PureBasic 5.72 (Windows - x64)
; ExecutableFormat = Console
; CursorPosition = 112
; FirstLine = 69
; Folding = --
; EnableXP
; Executable = ..\src\res\datafileMaker.exe