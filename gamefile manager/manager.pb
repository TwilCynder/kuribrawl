#FILEMARKER_DESCRIPTORSTART = $53
#FILEMARKER_ANIMSPEED = 1
#FILEMARKER_FRAMEINFO = 2
#FILEMARKER_FRAMEDURATION = $20
#FILEMARKER_FRAMEORIGIN = $21
#FILEMARKER_FRAMEMOVEMENT = $22
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
  If Not ReadFile(0, filename$)
    ProcedureReturn 0
  EndIf 
  *file\size = Lof(0)
  *file\buffer = ReAllocateMemory(*file\buffer, *file\size)
  ReadData(0, *file\buffer, *file\size)
  CloseFile(0)
  ProcedureReturn *file\buffer
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
        If Not ReadFile(2, infos)
          WriteByte(1, -1)
          ProcedureReturn 1
        EndIf 
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
                Select Left(value, 1)
                  Case "d"
                    WriteByte(1, #FILEMARKER_FRAMEDURATION)
                    WriteUnicodeCharacter(1, Val(Mid(value, 2)))
                    PrintN("- - - modified duration : " + Mid(value, 2))
                  Case "o"
                    WriteByte(1, #FILEMARKER_FRAMEORIGIN)
                    WriteLong(1, Val(Mid(value, 2)))
                    PrintN("- - - origin x : " + Mid(value, 2))
                    i + 1
                    WriteLong(1, Val(StringField(line, i, " ")))
                    PrintN("- - - origin x : " + StringField(line, i, " "))
                  Case "m"
                    WriteByte(1, #FILEMARKER_FRAMEMOVEMENT)
                    WriteByte(1, Val(Mid(value, 2)))
                    PrintN("- - - speed mode : " + Bin(Val(Mid(value, 2))))
                    i + 1
                    WriteDouble(1, ValD(StringField(line, i, " ")))
                    PrintN("- - - speed x : " + StringField(line, i, " "))
                    i + 1
                    WriteDouble(1, ValD(StringField(line, i, " ")))
                    PrintN("- - - speed y : " + StringField(line, i, " "))
                EndSelect
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
              WriteDouble(1, ValD(StringField(line, 6, " ")))
              WriteWord(1, Val(StringField(line, 7, " ")))
              WriteByte(1, Val(StringField(line, 8, " ")))
              WriteByte(1, Val(StringField(line, 9, " ")))
          EndSelect
        Wend 
        CloseFile(2)
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
  If Not readFileToMemory(path, *f)
    MessageRequester("error", "can't find file " + path)
    End
  EndIf 
  writeType(type)
  writeFileTag(tag)
  WriteFileLength(*f\size)
  PrintN("Pointer position : " + Str(Loc(1)))
  writeMemoryToFile(1, *f)
  writeFileDescriptor(type, info)
  writeInterfile()
EndProcedure

file.loadedFile
file\buffer = #Null

NewMap fileList.toLoad()

Define param.s, silent.b, buildPath.s
OpenConsole()

PrintN("start")

For i = 1 To CountProgramParameters()
  param = ProgramParameter()
  If Left(param, 1) = "-"
    If param = "-s"
      silent = 1
    EndIf
  ElseIf CheckFilename(param)
    SetCurrentDirectory(param)
    PrintN(GetCurrentDirectory())
  EndIf 
Next 


Define tag.s, path.s, type.b, infos.s
If Not ReadFile(2, "project_db.txt")
  PrintN("Can't find project DB")
  End
EndIf
If ReadFile(3, "buildinfo")
  buildPath = ReadString(3)
  CloseFile(3)
EndIf 

If buildPath = "" Or Not CreateFile(1, buildPath)
  CreateFile(1, "data.twl")
EndIf 
writeSignature()
writeVersion(0, 0, 1)

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

PrintN("Finished.")

If Not silent
  Input()
EndIf 



; IDE Options = PureBasic 5.72 (Windows - x64)
; ExecutableFormat = Console
; CursorPosition = 270
; FirstLine = 224
; Folding = --
; EnableXP
; UseIcon = ..\GraphicDesignIsMyPassion\iconDev.ico
; Executable = ..\src\res\datafileMaker.exe
; CommandLine = test/oui