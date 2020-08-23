;memo : penser à garder un array des frames (p*) crées pour pouvoir y accéder via leur n° (non-opti avec une simple liste)
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

Enumeration 
  #FILETYPE_ANIMATION
  #FILETYPE_LEFTANIM
  #FILETYPE_CHAMPION
EndEnumeration

Enumeration 
  #ANIMATIONTYPE_CHAMPION
  #ANIMATIONTYPE_STAGE
EndEnumeration

Procedure readChampionValues(*champion.Champion)
 *champion\walkSpeed = ReadDouble(0)
 *champion\dashSpeed = ReadDouble(0)
 *champion\initialDashSpeed = ReadDouble(0)
 *champion\dashTurnAccel = ReadDouble(0)
 *champion\maxAirSpeed = ReadDouble(0)
 *champion\airAcceleration = ReadDouble(0)
 *champion\traction = ReadDouble(0)
 *champion\jumpSpeed = ReadDouble(0) 
 *champion\jumpsquatDuration = ReadByte(0)
 *champion\dashStopDuration = ReadByte(0)
 *champion\dashStartDuration = ReadByte(0)
 *champion\dashTurnDuration = ReadByte(0)
 *champion\shortHopSpeed = ReadDouble(0)
 *champion\doubleJumpSpeed = ReadDouble(0)
 *champion\maxFallSpeed = ReadDouble(0)
 *champion\fastFallSpeed = ReadDouble(0)
 *champion\airFriction = ReadDouble(0)
 *champion\landingDuration = ReadByte(0)
EndProcedure  
  
Procedure.s LoadSprite_(*buffer, tag.s)
  Shared loadedSprites()
  Define sprite.l
  sprite = CatchSprite(#PB_Any, *buffer, #PB_Sprite_AlphaBlending)
  If Not sprite
    Debug "Could not load sprite " + tag
    End
  EndIf 
  loadedSprites(tag) = sprite
  ProcedureReturn tag
EndProcedure

Procedure makeFrames(*animation.AnimationModel, w.l, h.l, nb.b, Array *frames.FrameModel(1), originType)
  If Not *animation
    ProcedureReturn 0
  EndIf 
  Define x.l, y.l, w.l, h.l
  ReDim *frames(nb - 1)
  x = 0
  y = 0
  w = w / nb
  If originType = #ANIMATIONTYPE_CHAMPION
    For i = 0 To nb - 1
      *frames(i) = addFrame(*animation, x, y, w, h, w / 2, h)
      x + w
    Next
  Else 
    For i = 0 To nb - 1
      *frames(i) = addFrame(*animation, x, y, w, h, 0, 0)
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
  
Procedure loadGameData(path.s)
  Shared loadedSprites()
  ;- opening file
  If Not (ReadFile(0, path) And checkSignature())
    ProcedureReturn 0
  EndIf 
  readVersion()

  Define type.b, tag.s, size.l, *buffer, byte.b, *animation.AnimationModel, *character.Champion, selectedElement.b, value.l, w.l, h.l, valueF.f, subType.b
  Dim *frames.FrameModel(0)
  
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
          Debug "Error : unexpected byte (no descriptorstart at the beginning of a champion data)"
        EndIf 
        *character = tryChampion(tag)
        *character\displayName = ReadString(0, #PB_UTF8)
        readChampionValues(*character)  
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
            Case #FILEMARKER_FRAMEDURATION
              *frames(selectedElement)\duration = ReadUnicodeCharacter(0)
            Case #FILEMARKER_INTERFILE
              Break 
          EndSelect
        ForEver
      Case #FILETYPE_ANIMATION
        ;- - File type : Animation
        ;- - - Parsing animation tag
        character.s = StringField(tag, 1, "/")
        animationName.s = StringField(tag, 2, "/")
        
        ;- - - Loading spritesheet
        LoadSprite_(*buffer, tag)
        
        *animation = 0
        
        ;- - - Obtaining animation object
        If (Left(character, 1) = "_")
          subType = #ANIMATIONTYPE_STAGE
          character = Mid(character, 2)
          *stage.StageModel = getStage(character)
          *animation = newStageAnimation(*stage, animationName, tag)
          *animation\noCollisions = 1
          If animationName = "background"
            *stage\backgroundAnim = *animation
          EndIf 
        Else
          subType = #ANIMATIONTYPE_CHAMPION
          *animation = getAnimation(tryChampion(character), animationName)
          If Not *animation
            *animation = newAnimation(tryChampion(character), animationName, tag)
          Else
            *animation\spriteSheet = loadedSprites(tag)
          EndIf 
        EndIf 
          
        w = SpriteWidth(loadedSprites(tag))
        h = SpriteHeight(loadedSprites(tag))
        
        byte = ReadByte(0)
        If byte = #FILEMARKER_INTERFILE
          Continue
        ElseIf Not byte = #FILEMARKER_DESCRIPTORSTART
          Debug "Error : unexpected byte (neither interfile or descriptorstart) after file " + tag
          End
        EndIf 
        ;- - - reading descriptor
        
        byte = ReadByte(0)
        
        If byte > 0
          makeFrames(*animation, w, h, byte, *frames(), subType)
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
              *frames(selectedElement)\duration = ReadUnicodeCharacter(0)
            Case #FILEMARKER_FRAMEORIGIN
              *frames(selectedElement)\origin\x = ReadLong(0)
              *frames(selectedElement)\origin\y = ReadLong(0)
            Case #FILEMARKER_FRAMEMOVEMENT
              *frames(selectedElement)\speedMode = ReadByte(0)
              *frames(selectedElement)\speed\x = ReadDouble(0)
              *frames(selectedElement)\speed\y = ReadDouble(0)
            Case #FILEMARKER_HITBOXINFO
              selectedElement = ReadByte(0)
              x.l = ReadWord(0)
              y.l = ReadWord(0)
              w.l = ReadWord(0)
              h.l = ReadWord(0)
              addHitbox(*frames(selectedElement), x, y, w, h)
              *frames(selectedElement)\hitboxes()\damage = ReadDouble(0)
              *frames(selectedElement)\hitboxes()\angle = ReadWord(0)
              *frames(selectedElement)\hitboxes()\priority = ReadByte(0)
              *frames(selectedElement)\hitboxes()\hit = ReadByte(0)
            Case #FILEMARKER_HURTBOXINFO
              selectedElement = ReadByte(0)
              x.l = ReadWord(0)
              If x = $5454
                x = - *frames(selectedElement)\origin\x
                y = - *frames(selectedElement)\origin\y
                w = *frames(selectedElement)\display\w
                h = *frames(selectedElement)\display\h
              Else
                y.l = ReadWord(0)
                w.l = ReadWord(0)
                h.l = ReadWord(0)
              EndIf 
              addHurtbox(*frames(selectedElement), x, y, w, h)
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
    EndSelect  
  Until Eof(0)

  CloseFile(0)
  ProcedureReturn 1
EndProcedure

UsePNGImageDecoder()
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 135
; FirstLine = 88
; Folding = -
; EnableXP