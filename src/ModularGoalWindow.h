#include "gbafe.h"

typedef struct ROMChapterData ROMChapterData;
typedef struct goalOptions goalOptions;
typedef struct GoalWindowProc GoalWindowProc;
typedef struct ClockTextProc ClockTextProc;

struct GoalWindowProc {
    /* 00 */ PROC_HEADER;

    /* 2A */ u16 pad2A;
    /* 2C */ TextHandle textA;
    /* 34 */ TextHandle textB;
    /* 3C */ u8 pad3C[0x44 - 0x3C];

    /* 44 */ u16 bottomPadding;

    /* 46 */ u8 pad46[0x4C - 0x46];

    /* 4C */ s8 unk4C;
    /* 4D */ s8 unk4D;
    /* 4E */ s8 unk4E;
    /* 4F */ s8 unk4F;
    /* 50 */ s8 unk50;

    /* 51 */ u8 pad51[0x55 - 0x51];

    /* 55 */ s8 unk55;
    /* 56 */ s8 unk56;
    /* 57 */ s8 unk57;
    /* 58 */ int clock;
};

struct ClockTextProc {
    /* 00 */ PROC_HEADER;
    /* 29 */ u8 x;
    /* 2A */ u16 clock;
    /* 2C */ TextHandle* text;
};

struct goalOptions {
    /* 00 */ u8 charIndex;
    /* 01 */ u8 charColor;
    /* 02 */ u8 charX;
    /* 03 */ u8 charY;
    /* 04 */ u8 iconIndex;
    /* 05 */ u8 iconPal;
    /* 06 */ u8 iconX;
    /* 07 */ u8 iconY;
};

struct textIndexes {
    /* 00 */ u16 indexA;
    /* 04 */ u16 indexB;
};

//Draw Time Text
void StartClockText(TextHandle* text, int x, GoalWindowProc* parent);
void UpdateClockText(ClockTextProc* proc);

extern int CountUnitsByFaction(u8 factionBit);

extern struct goalOptions MGWOptions[0xFF];
extern struct textIndexes MGWText[0xFF];

static u16 const* const TSA_BIGWINDOW = (u16 const*) 0x08A176BC;
static u16 const* const TSA_SMALLWINDOW = (u16 const*) 0x08A17744;

static u16* const TM_BACKGROUND = (u16*) 0x020044D4;
static u16* const TM_FOREGROUND = (u16*) 0x02004054;

