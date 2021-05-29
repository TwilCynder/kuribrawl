;TODO Modifs in the file format
;- Change the $5x filemarkers to a more meaningful value
;- Animation speed : float->double (DONE)

Structure File
    path.s
    content.s
EndStructure

Structure LoadedFile
    size.l
    *buffer
EndStructure

Enumeration 
    #FILETYPE_ANIMATION
    #FILETYPE_LEFTANIM
    #FILETYPE_CHAMPION
    #FILETYPE_STAGE
    #FILETYPE_IMAGE
    #FILETYPES
EndEnumeration

#FILEMARKER_404 = $FD
#FILEMARKER_INTERFILE = $FF
#FILEMARKER_DESCRIPTORSTART = $FE
#FILEMARKER_ANIMSPEED = 1
#FILEMARKER_FRAMEINFO = 2
#FILEMARKER_FRAMEDURATION = $20
#FILEMARKER_FRAMEORIGIN = $21
#FILEMARKER_FRAMEMOVEMENT = $22
#FILEMARKER_HURTBOXINFO = 3
#FILEMARKER_HITBOXINFO = 4
#FILEMARKER_MOVEINFO = 2
#FILEMARKER_LANDINGLAG = $20
#FILEMARKER_MULTIMOVE = 3
#FILEMARKER_MULTIMOVEEND = $30
#FILEMARKER_PLATFORMINFO = 1

Dim filetypeNames.s(#FILETYPES - 1)

filetypeNames(#FILETYPE_ANIMATION) = "Animation"
filetypeNames(#FILETYPE_CHAMPION) = "Champion"
filetypeNames(#FILETYPE_LEFTANIM) = "Left Animation"
filetypeNames(#FILETYPE_IMAGE) = "Simple image"
filetypeNames(#FILETYPE_STAGE) = "Stage"

NewList files.File()

Define enableMessageBox.b = #False, logging.b = #True

If Not logging
    enableMessageBox = #True
EndIf 

Procedure printLog(text.s)
    Shared logging
    If logging
        PrintN(text)
    EndIf 
EndProcedure

Procedure errorPopup(text.s)
    Shared enableMessageBox
    printLog("Error : " + text)
    If enableMessageBox
        MessageBox_(0, "Kuribrawl Data File Maker error : " + Chr(13) + text, "Error", #MB_OK | #MB_ICONERROR)
    EndIf 
    EnableASM
EndProcedure

Procedure writeSignature(datafile.l)
    WriteLong(datafile, $54545454)
EndProcedure

Procedure writeVersion(datafile.l, maj.a, min.a, rev.a)
    WriteAsciiCharacter(datafile, maj)
    WriteAsciiCharacter(datafile, min)
    WriteAsciiCharacter(datafile, rev)
EndProcedure  

Procedure readFileList()
    Shared files()
    
    file.l = ReadFile(#PB_Any, "project_db.txt")
    If Not file
        errorPopup("Could Not file project DB.")
        End
    EndIf
    
    While Not Eof(file)
        AddElement(files())
        files()\path = ReadString(file)
        files()\content = ReadString(file)
    Wend
    CloseFile(file)
EndProcedure

loadedFile.LoadedFile
loadedFile\buffer = #Null
loadedFile\size = 0
Procedure readFileToMemory(path.s)
    Shared loadedFile
    file.l = ReadFile(#PB_Any, path)
    If Not file
        ProcedureReturn 0
    EndIf
    
    loadedFile\size = Lof(file)
    loadedFile\buffer = ReAllocateMemory(loadedFile\buffer, loadedFile\size)
    
    ReadData(file, loadedFile\buffer, loadedFile\size)
    CloseFile(file)
    ProcedureReturn loadedFile\size
EndProcedure

Procedure writeMemoryToFile(datafile.l)
    Shared loadedFile
    WriteData(datafile, loadedFile\buffer, loadedFile\size)
EndProcedure

Procedure writeFileType(datafile.l, type.b)
    WriteAsciiCharacter(datafile, type)
EndProcedure

Procedure writeFileTag(datafile.l, tag.s)
    WriteString(datafile, tag)
    WriteAsciiCharacter(datafile, 0)
EndProcedure

Procedure writeAnimationDescriptor(datafile.l, info.s)
    Define value.l, line.s, value$
    
    WriteAsciiCharacter(datafile, #FILEMARKER_DESCRIPTORSTART)
    printLog("Writing descriptor (format : animation)")
    If Right(info, 4) = ".dat"
        printLog("- Uses a descriptor file : " + info)
        descriptorFile.l = ReadFile(#PB_Any, info)
        If Not descriptorFile
            errorPopup("Could not open descriptor file " + info)
        EndIf 
        
        
    Else 
        value$ = StringField(info, 1, " ")
        If Not value
            errorPopup("Missing frame number")
        EndIf 
        
        value = Val(value$)
        If value < 1  
            errorPopup("Null or negative frame number")
        EndIf 
        WriteByte(datafile, value)
        printLog("- Frames number : " + value$)
        
        value$ = StringField(info, 2, " ")
        value = ValD(value$)
        If value
          WriteAsciiCharacter(1, #FILEMARKER_ANIMSPEED)
          WriteDouble(1, value)
          printLog("- Speed" + value$)
        EndIf 
    EndIf 
EndProcedure

Procedure addFile(datafile.l, *inputFile.File)
    Shared filetypeNames()
    Define type.b
        
    printLog("===================")
    printLog("Filename : " + *inputFile\path)
    printLog("Content  : " + *inputFile\content)
    
    Select StringField(*inputFile\content, 1, ":")
        Case "A"
            type = #FILETYPE_ANIMATION
        Case "AL"
            type = #FILETYPE_LEFTANIM
        Case "C"
            type = #FILETYPE_CHAMPION
        Case "S"
            type = #FILETYPE_STAGE
        Case "I"
            type = #FILETYPE_IMAGE
    EndSelect
    
    printLog("Type : " + filetypeNames(type))
    
    Define tag.s, info.s
    tag  = StringField(*inputFile\content, 1, " ")
    info = StringField(*inputFile\content, 2, " ")
    
    printLog("Tag  : " + tag)
    printLog("Info : " + info)
    
    writeFileType(datafile, type)
    writeFileTag(datafile, tag)
    
    If type = #FILETYPE_ANIMATION Or type = #FILETYPE_LEFTANIM Or type = #FILETYPE_IMAGE
        size.l = readFileToMemory(*inputFile\path)
        If Not size
            errorPopup("Could not load file " + *inputFile\path)
            End
        EndIf 
        WriteInteger(datafile, size)
        writeMemoryToFile(datafile)
        If type = #FILETYPE_ANIMATION
            writeAnimationDescriptor(datafile, info)
        EndIf
    EndIf 
EndProcedure

buildpath.s = "data.twl"

If Not CreateFile(0, buildpath)
    errorPopup("Could not create the Data File.")
    End
EndIf

If logging
    OpenConsole()
EndIf     
    
writeSignature(0)
writeVersion(0, 0, 2, 0)

readFileList()

ForEach files()
    addFile(0, @files())
Next 

If logging
    Input()
EndIf 
; IDE Options = PureBasic 5.72 (Windows - x64)
; ExecutableFormat = Console
; CursorPosition = 142
; FirstLine = 123
; Folding = --
; EnableXP