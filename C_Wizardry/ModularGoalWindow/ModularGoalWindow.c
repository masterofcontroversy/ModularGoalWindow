
#include "gbafe.h"

enum { TEXT_SPECIAL_G = 30 };


struct goalOptions {
    u8      charIndex;
    u8      charColor;
    u8      charX;
    u8      charY;
    u8      iconIndex;
    u8      iconPal;
    u8      iconX;
    u8      iconY;
};

extern struct ROMChapterData* GetChapterDefinition(int index);
extern struct goalOptions MGWOptions[0xFF];

struct PiProc {
	/* 00 */ PROC_HEADER;

    /* 2A */ u16 pad2A;
	/* 2C */ struct TextHandle textA;
	/* 34 */ struct TextHandle textB;
	/* 3C */ u8 pad3C[0x44 - 0x3C];

	/* 44 */ short bottomPadding;

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

static u16 const* const TSA_BIGWINDOW = (u16 const*) 0x08A176BC;
static u16 const* const TSA_SMALLWINDOW = (u16 const*) 0x08A17744;

static u16* const TM_BACKGROUND = (u16*) 0x020044D4;
static u16* const TM_FOREGROUND = (u16*) 0x02004054;

void NuPiPutFrame(struct PiProc* proc) {

	BgMapFillRect(TM_BACKGROUND, 11, 9, 0);
	BgMapFillRect(TM_FOREGROUND, 11, 9, 0);

    Text_Display(&proc->textA, TM_FOREGROUND + TILEMAP_INDEX(1, 1));
    
    if (proc->bottomPadding > 0) {
        BgMap_ApplyTsa(TM_BACKGROUND, TSA_BIGWINDOW, TILEREF(0, 1));
        Text_Display(&proc->textB, TM_FOREGROUND + TILEMAP_INDEX(1, 3));
    }
    else {
        BgMap_ApplyTsa(TM_BACKGROUND, TSA_SMALLWINDOW, TILEREF(0, 1));
    }
    
    int WindowType = GetChapterDefinition(gChapterData.chapterIndex)->goalWindowDataType;
    
    if (MGWOptions[WindowType].charIndex != 0) {
        DrawSpecialUiChar(TM_FOREGROUND + TILEMAP_INDEX(MGWOptions[WindowType].charX, MGWOptions[WindowType].charY), MGWOptions[WindowType].charColor, MGWOptions[WindowType].charIndex);
    }
    if (MGWOptions[WindowType].iconIndex != 0xFF) {
        DrawIcon(TM_FOREGROUND + TILEMAP_INDEX(MGWOptions[WindowType].iconX, MGWOptions[WindowType].iconY), MGWOptions[WindowType].iconIndex, 0xD000);
        LoadIconPalette(MGWOptions[WindowType].iconPal, 0xD);
    }
    
}

void DrawGoldTotal(struct TextHandle* handle, int color) {
    Text_SetColorId(handle, color);
    Text_DrawNumber(handle, GetPartyGoldAmount());
}

//Display gold only
void GoldTextTemplate(struct PiProc* proc) {
    Text_Clear(&proc->textA);
    Text_SetXCursor(&proc->textA, 0x36);
    DrawGoldTotal(&proc->textA, TEXT_COLOR_BLUE);
    
    proc->bottomPadding = 0;
}

//Display gold and current turn
void GoldTurnTextTemplate(struct PiProc* proc) {
    Text_Clear(&proc->textA);
    Text_SetXCursor(&proc->textA, 0x36);
    //DrawGoldTotal(&proc->textA, TEXT_COLOR_BLUE);
    
    Text_Clear(&proc->textB);
    Text_InsertString(&proc->textB, 0x6, TEXT_COLOR_NORMAL, "Turn:");
    Text_SetXCursor(&proc->textB, 0x36);
    Text_SetColorId(&proc->textB, TEXT_COLOR_BLUE);
    Text_DrawNumber(&proc->textB, gChapterData.turnNumber);
    
    proc->bottomPadding = 1;
}
