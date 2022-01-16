
#include "gbafe.h"

enum { TEXT_SPECIAL_G = 30 };

struct PiProc
{
	/* 00 */ PROC_HEADER;

             u16 pad2A;
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

void NuPiPutFrame(struct PiProc* proc) {
    static u16 const* const TSA_BIGWINDOW = (u16 const*) 0x08A176BC;
	static u16 const* const TSA_SMALLWINDOW = (u16 const*) 0x08A17744;

	static u16* const TM_BACKGROUND = (u16*) 0x020044D4;
	static u16* const TM_FOREGROUND = (u16*) 0x02004054;

	BgMapFillRect(TM_BACKGROUND, 11, 9, 0);
	BgMapFillRect(TM_FOREGROUND, 11, 9, 0);

    Text_Display(&proc->textA, TM_FOREGROUND + TILEMAP_INDEX(1, 1));
    
    if (proc->bottomPadding > 0) {
        BgMap_ApplyTsa(TM_BACKGROUND, TSA_BIGWINDOW, TILEREF(0, 3));
        Text_Display(&proc->textB, TM_FOREGROUND + TILEMAP_INDEX(1, 3));
    }
    else {
        BgMap_ApplyTsa(TM_BACKGROUND, TSA_SMALLWINDOW, TILEREF(0, 1));
    }
    

	//DrawUiNumber(TM_FOREGROUND + TILEMAP_INDEX(8, 1), TEXT_COLOR_BLUE, GetPartyGoldAmount());
	DrawSpecialUiChar(TM_FOREGROUND + TILEMAP_INDEX(9, 1), TEXT_COLOR_GOLD, TEXT_SPECIAL_G);
}
    
void TextTemplate1(struct PiProc* proc) {
    Text_Clear(&proc->textA);
    Text_InsertString(&proc->textA, 0x6, TEXT_COLOR_NORMAL, "Fire Emblem");
    
    Text_InsertString(&proc->textB, 0x6, TEXT_COLOR_BLUE, "is cringe");
    
    proc->bottomPadding = 1;
}
