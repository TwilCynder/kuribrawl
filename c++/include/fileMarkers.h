#pragma once
//General
#define FILEMARKER_404 0xF0
#define FILEMARKER_INTERFILE 0xFF
#define FILEMARKER_DESCRIPTORSTART 0xFE
#define FILEMARKER_GENERICSEPARATOR 0xFD

//Animations
#define FILEMARKER_ANIMSPEED 1
#define FILEMARKER_FRAMEINFO 2
#define FILEMARKER_FRAMEDURATION 0x20
#define FILEMARKER_FRAMEORIGIN 0x21
#define FILEMARKER_FRAMEMOVEMENT 0x22
#define FILEMARKER_HURTBOXINFO 3
#define FILEMARKER_HURTBOXFRAMEINDEX 0x31
#define FILEMARKER_HITBOXINFO 4
#define FILEMARKER_HITBOXFRAMEINDEX 0x41
//Champions
#define FILEMARKER_MOVEINFO 2
#define FILEMARKER_LANDINGLAG 0x20
#define FILEMARKER_MULTIMOVE 3
#define FILEMARKER_MULTIMOVEEND 0x30
//Stage
#define FILEMARKER_PLATFORMINFO = 1

#define MAX_VALUE_BYTE  255
#define MAX_VALUE_SHORT 65535
#define MAX_VALUE_USHORT 32767