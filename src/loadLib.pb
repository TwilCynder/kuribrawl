#FILEMARKER_DESCRIPTORSTART = $53
#FILEMARKER_ANIMSPEED = 1
#FILEMARKER_FRAMEINFO = 2
#FILEMARKER_FRAMEDURATION = $20
#FILEMARKER_FRAMEORIGIN = $21
#FILEMARKER_FRAMEMOVEMENT = $22
#FILEMARKER_HURTBOXINFO = 3
#FILEMARKER_HITBOXINFO = 4
#FILEMARKER_INTERFILE = $54
#FILEMARKER_MOVEINFO = 2
#FILEMARKER_LANDINGLAG = $20
#FILEMARKER_MULTIMOVE = 3
#FILEMARKER_MULTIMOVEEND = $30
#FILEMARKER_PLATFORMINFO = 1

Enumeration 
  #FILETYPE_ANIMATION
  #FILETYPE_LEFTANIM
  #FILETYPE_CHAMPION
  #FILETYPE_STAGE
  #FILETYPE_IMAGE
EndEnumeration

Enumeration 
  #ANIMATIONTYPE_CHAMPION
  #ANIMATIONTYPE_STAGE
EndEnumeration

#MAX_VALUE_BYTE = -127
#MAX_VALUE_DOUBLE = %111111111111111111111111111111111111111111111111111111111111111

Procedure skipToNextInterfile()
  While Not ReadByte(0) = #FILEMARKER_INTERFILE
  Wend  
EndProcedure

Procedure readChampionValues(*champion.Champion)
  *champion\weight = ReadDouble(0)
  *champion\walkSpeed = ReadDouble(0)
  *champion\dashSpeed = ReadDouble(0)
  *champion\initialDashSpeed = ReadDouble(0)
  *champion\dashTurnAccel = ReadDouble(0)
  *champion\maxAirSpeed = ReadDouble(0)
  *champion\airAcceleration = ReadDouble(0)
  *champion\airFriction = ReadDouble(0)
  *champion\traction = ReadDouble(0)
  *champion\jumpSpeed = ReadDouble(0) 
  *champion\shortHopSpeed = ReadDouble(0)
  *champion\doubleJumpSpeed = ReadDouble(0)
  *champion\maxFallSpeed = ReadDouble(0)
  *champion\fastFallSpeed = ReadDouble(0)
  *champion\jumpsquatDuration = ReadByte(0)
  *champion\dashStopDuration = ReadByte(0)
  *champion\dashStartDuration = ReadByte(0)
  *champion\dashTurnDuration = ReadByte(0)
  *champion\landingDuration = ReadByte(0)
  *champion\shieldStartup = ReadByte(0)
  *champion\shieldEndlag = ReadByte(0)
  *champion\shieldInfo\x = ReadByte(0)
  *champion\shieldInfo\y = ReadByte(0)
  *champion\shieldInfo\size = ReadByte(0)
EndProcedure  

Procedure adaptChampionValues(*champion.Champion)
  If *champion\shieldStartup = #MAX_VALUE_BYTE
    *champion\shieldStartup = kuribrawl\ingameData\shieldStartup
  EndIf 
  If *champion\shieldEndlag = #MAX_VALUE_BYTE
    *champion\shieldEndlag = kuribrawl\ingameData\shieldEndlag
  EndIf
EndProcedure

Procedure readStageValues(*stage.StageModel)
  *stage\w = ReadWord(0)
  *stage\h = ReadWord(0)
  camera.Rect_
  camera\x = ReadWord(0)
  camera\y = ReadWord(0)
  camera\w = ReadWord(0)
  camera\h = ReadWord(0)
  setStageCameraZone(*stage, camera\x, camera\y, camera\w, camera\h)
EndProcedure

Procedure LoadSprite_(*buffer, tag.s)
  Shared loadedSprites()
  Define sprite.l
  sprite = CatchSprite(#PB_Any, *buffer, #PB_Sprite_AlphaBlending)
  If Not sprite
    Debug "Could not load sprite " + tag
    End
  EndIf 
  loadedSprites(tag) = sprite
  ProcedureReturn sprite
EndProcedure

Procedure makeFrames(*animation.AnimationModel, w.l, h.l, nb.b, animType.b)
  If Not *animation
    Debug "makeframes : Animation is null"
  EndIf 
  setAnimationFrameNumber(*animation, nb)
  Define x.l, y.l, w.l, h.l
  x = 0
  y = 0
  w = w / nb
  If animType = #ANIMATIONTYPE_CHAMPION
    For i = 0 To nb - 1
      setFrameProperties(*animation\frames(i), x, y, w, h, w / 2, h)
      x + w
    Next
  Else 
    For i = 0 To nb - 1
      setFrameProperties(*animation\frames(i), x, y, w, h, 0, h)
      x + w
    Next
  EndIf 
EndProcedure

Procedure tryChampion(name.s)
  *r = getCharacter(name)
  If Not *r
    *r = newCharacter(name)
  EndIf
  ProcedureReturn *r
EndProcedure  

Procedure tryStage(name.s)
  *r = getStage(name)
  If Not *r
    *r = newStage(name)
  EndIf 
  ProcedureReturn *r
EndProcedure

Procedure selectFighterAnimationInfo(*animation.FighterAnimationModel, value, selectedElement.b)
  Select value
    Case #FILEMARKER_FRAMEMOVEMENT
      *animation\frames(selectedElement)\speedMode = ReadByte(0)
      *animation\frames(selectedElement)\speed\x = ReadDouble(0)
      *animation\frames(selectedElement)\speed\y = ReadDouble(0)
    Case #FILEMARKER_HITBOXINFO
      selectedElement = ReadByte(0)
      x.l = ReadWord(0)
      y.l = ReadWord(0)
      w.l = ReadWord(0)
      h.l = ReadWord(0)
      addHitbox(*animation\frames(selectedElement), x, y, w, h)
      *animation\frames(selectedElement)\hitboxes()\damage = ReadFloat(0)
      *animation\frames(selectedElement)\hitboxes()\angle = ReadWord(0)
      *animation\frames(selectedElement)\hitboxes()\bkb = ReadFloat(0)
      *animation\frames(selectedElement)\hitboxes()\skb = ReadFloat(0)
      *animation\frames(selectedElement)\hitboxes()\priority = ReadByte(0)
      *animation\frames(selectedElement)\hitboxes()\hit = ReadByte(0)
    Case #FILEMARKER_HURTBOXINFO
      selectedElement = ReadByte(0)
      x.l = ReadWord(0)
      If x = $5454
        x = - *animation\frames(selectedElement)\origin\x
        y = - *animation\frames(selectedElement)\origin\y
        w = *animation\frames(selectedElement)\display\w
        h = *animation\frames(selectedElement)\display\h
      Else
        y.l = ReadWord(0)
        w.l = ReadWord(0)
        h.l = ReadWord(0)
      EndIf
      addHurtbox(*animation\frames(selectedElement), x, y, w, h)
    Default 
      error("Loading : unexpected filemarker byte " + Str(byte) + " in animation descriptor For " + tag + " at " + Hex(Loc(0)))
  EndSelect
EndProcedure

Procedure loadGameData(path.s)
  Shared loadedSprites()
  ;- opening file
  If Not (ReadFile(0, path) And checkSignature())
    ProcedureReturn 0
  EndIf 
  readVersion()

  Define sprite.l, type.b, tag.s, size.l, *buffer, byte.b, *character.Champion, selectedElement.b, value.l, x.l, y.l, w.l, h.l, valueF.f, subType.b, value$
  
  *buffer = 0
  Repeat
    ;- reading file info
    type = readFileType()
    tag = readFileTag()
    size = readFileLength()
    *buffer = readFileData(size, *buffer)
    
    ;-reading file data
    Select type
      Case #FILETYPE_CHAMPION
        If Not ReadByte(0) = #FILEMARKER_DESCRIPTORSTART
          PrintN("Error : unexpected byte (no descriptorstart at the beginning of a champion data)")
          skipToNextInterfile()
          Continue
        EndIf 
        *character = tryChampion(tag)
        *character\displayName = ReadString(0, #PB_UTF8)
        readChampionValues(*character)  
        adaptChampionValues(*character)
        Repeat 
          byte = ReadByte(0)
          Select byte
            Case #FILEMARKER_MOVEINFO
              selectedElement = ReadByte(0)
            Case #FILEMARKER_LANDINGLAG
              *character\moves(selectedElement)\landLag = ReadByte(0)
            Case #FILEMARKER_MULTIMOVE
              selectedElement = ReadByte(0)
              NewList multiMove.b()
              byte = ReadByte(0)
              value = 0
              While Not byte = #FILEMARKER_MULTIMOVEEND
                addElementVal(multiMove(), byte)
                byte = ReadByte(0)
                addElementVal(multiMove(), byte)
                byte = ReadByte(0)
                value + 1
              Wend 
              makeMultiMove(*character\moves(selectedElement), value)
              value = 0
              ForEach multiMove()
                *character\moves(selectedElement)\multiMove\partEndFrames(value) = multiMove()
                NextElement(multiMove())
                *character\moves(selectedElement)\multiMove\partStartFrames(value) = multiMove()
                value + 1
              Next 
            Case #FILEMARKER_INTERFILE
              Break 
          EndSelect
        ForEver
      Case #FILETYPE_STAGE
        If Not ReadByte(0) = #FILEMARKER_DESCRIPTORSTART
          PrintN("Error : unexpected byte (no descriptorstart at the beginning of a stage data)")
          skipToNextInterfile()
          Continue
        EndIf 
        *stage.StageModel = tryStage(tag)
        *stage\displayName = ReadString(0, #PB_UTF8)
        readStageValues(*stage)  
        Repeat 
          byte = ReadByte(0)
          Select byte
            Case #FILEMARKER_PLATFORMINFO
              x.l = ReadWord(0)
              y.l = ReadWord(0)
              w.l = ReadWord(0)
              value$ = ReadString(0, #PB_Ascii)
              If x = -1
                addCenteredPlatform(*stage, y, w, value$)
              Else
                addPlatform(*stage, x, y, w, value$)
              EndIf   
            Case #FILEMARKER_INTERFILE
              Break 
          EndSelect
        ForEver
      Case #FILETYPE_ANIMATION
        ;- - File type : Animation
        ;- - - Parsing animation tag
        namespace.s = StringField(tag, 1, "/")
        animationName.s = StringField(tag, 2, "/")
        
        ;- - - Loading spritesheet
        LoadSprite_(*buffer, tag)
        
        *animation.AnimationModel = 0
        
        If Left(namespace, 1) = "_"
          subType = #ANIMATIONTYPE_STAGE
          namespace = Mid(namespace, 2)
          *stage.StageModel = tryStage(namespace)
          *animation = newStageAnimation(*stage, animationName, tag)
          If animationName = "background"
            *stage\backgroundAnim = *animation
          EndIf
        Else
          subType = #ANIMATIONTYPE_CHAMPION
          *animation = getAnimation(tryChampion(namespace), animationName)
          If Not *animation
            *animation = newAnimation(tryChampion(namespace), animationName, tag)
          Else
            *animation\spriteSheet = loadedSprites(tag)
          EndIf 
        EndIf   
        
        byte = ReadByte(0)
        If byte = #FILEMARKER_INTERFILE
          Continue
        ElseIf Not byte = #FILEMARKER_DESCRIPTORSTART
          Debug "Error : unexpected byte (neither interfile or descriptorstart) after file " + tag
          End
        EndIf 
        ;- - - reading descriptor
        
        w = SpriteWidth(loadedSprites(tag))
        h = SpriteHeight(loadedSprites(tag))
        
        byte = ReadByte(0)
        
        If byte > 0
          makeFrames(*animation, w, h, byte, subType)
        EndIf 
 
        Repeat 
          byte = ReadByte(0)
          Select byte
            Case #FILEMARKER_ANIMSPEED
              valueF = ReadFloat(0)
              *animation\baseSpeed = valueF
            Case #FILEMARKER_INTERFILE
              Break 
            Case #FILEMARKER_FRAMEINFO
              selectedElement = ReadByte(0)
            Case #FILEMARKER_FRAMEDURATION
              *animation\frames(selectedElement)\duration = ReadUnicodeCharacter(0)
            Case #FILEMARKER_FRAMEORIGIN
              *animation\frames(selectedElement)\origin\x = ReadLong(0)
              *animation\frames(selectedElement)\origin\y = ReadLong(0)
            Default 
              If subType = #ANIMATIONTYPE_CHAMPION
                selectFighterAnimationInfo(*animation, byte, selectedElement)
              Else
                error("Loading : unexpected tag byte " + Str(byte) + " in animation descriptor For " + tag + " at " + Hex(Loc(0)))
              EndIf 
          EndSelect
        ForEver
        
        
        If *animation\baseSpeed = 0
          *animation\baseSpeed = 1
        EndIf 
      Case #FILETYPE_LEFTANIM
        character.s = StringField(tag, 1, "/")
        animationName.s = StringField(tag, 2, "/")
        tag = tag + "L"
        
        LoadSprite_(*buffer, tag)
        
        *animation = getAnimation(tryChampion(character), animationName)
        If Not *animation
          *animation = newAnimation(tryChampion(character), animationName, "")
        EndIf
        
        addLeftSpritesheet(getAnimation(getCharacter(character), animationName), tag)
        checkInterfile()
      Case #FILETYPE_IMAGE
        Debug tag
        sprite = LoadSprite_(*buffer, tag)
        checkInterfile()
      Default
        Debug "Error at " + Hex(Loc(0)) + " : unexpected byte (not a filetype)"
        skipToNextInterfile()
    EndSelect  
  Until Eof(0)
  
  CloseFile(0)
  ProcedureReturn 1
EndProcedure

UsePNGImageDecoder()
; IDE Options = PureBasic 5.72 (Windows - x64)
<<<<<<< Updated upstream
; CursorPosition = 68
; FirstLine = 64
=======
; CursorPosition = 164
; FirstLine = 137
>>>>>>> Stashed changes
; Folding = --
; EnableXP