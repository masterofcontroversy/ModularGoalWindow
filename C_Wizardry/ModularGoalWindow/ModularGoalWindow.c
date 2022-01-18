#include "gbafe.h"

struct goalOptions {
    u8 charIndex;
    u8 charColor;
    u8 charX;
    u8 charY;
    u8 iconIndex;
    u8 iconPal;
    u8 iconX;
    u8 iconY;
};

struct textIndexes {
    u16 indexA;
    u16 indexB;
};

//Vanilla function for getting ROM chapter data
extern struct ROMChapterData* GetChapterDefinition(int index);

//Byte used for the icon ID
extern u8 FreeByte;

//Options and textIDs set in EA
extern struct goalOptions MGWOptions[0xFF];
extern struct textIndexes MGWText[0xFF];

struct PiProc {
	/* 00 */ PROC_HEADER;

    /* 2A */ u16 pad2A;
	/* 2C */ struct TextHandle textA;
	/* 34 */ struct TextHandle textB;
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

static u16 const* const TSA_BIGWINDOW = (u16 const*) 0x08A176BC;
static u16 const* const TSA_SMALLWINDOW = (u16 const*) 0x08A17744;

static u16* const TM_BACKGROUND = (u16*) 0x020044D4;
static u16* const TM_FOREGROUND = (u16*) 0x02004054;

void DrawGoalWindow(struct PiProc* proc) {

	BgMapFillRect(TM_BACKGROUND, 11, 9, 0);
	BgMapFillRect(TM_FOREGROUND, 11, 9, 0);

    Text_Display(&proc->textA, TM_FOREGROUND + TILEMAP_INDEX(1, 1));
    
    if (proc->bottomPadding != 0) {
        BgMap_ApplyTsa(TM_BACKGROUND, TSA_BIGWINDOW, TILEREF(0, 1));
        Text_Display(&proc->textB, TM_FOREGROUND + TILEMAP_INDEX(1, 3));
    }
    else {
        BgMap_ApplyTsa(TM_BACKGROUND, TSA_SMALLWINDOW, TILEREF(0, 1));
    }
    
    int gwIndex = GetChapterDefinition(gChapterData.chapterIndex)->goalWindowDataType;
    
    //If the byte that decides if the template is using a speical character is 0, don't draw any special character
    //Character specs are determined in EA
    if (MGWOptions[gwIndex].charIndex != 0) {
        DrawSpecialUiChar(TM_FOREGROUND + TILEMAP_INDEX(MGWOptions[gwIndex].charX, MGWOptions[gwIndex].charY), MGWOptions[gwIndex].charColor, MGWOptions[gwIndex].charIndex);
    }
    //TODO: Fix the palette problem
    //If either the byte that decides if the template is using an icon is set to 0, or the RAM byte that determines what icon to display is 0xFF, don't draw any icon
    //Icon location is determined in EA
    if (MGWOptions[gwIndex].iconIndex != 0 && FreeByte != 0xFF) {
        DrawIcon(TM_FOREGROUND + TILEMAP_INDEX(MGWOptions[gwIndex].iconX, MGWOptions[gwIndex].iconY), FreeByte, 0xD000);
        LoadIconPalette(MGWOptions[gwIndex].iconPal, 0xD);
    }
    
}

//Ease of use function for inserting numbers in textHandles
void InsertNumber(struct TextHandle* handle, int xCursor, int color, int number) {
    Text_SetXCursor(handle, xCursor); //XCursor is how far to the right your text is located
    Text_SetColorId(handle, color); //Color of the text
    Text_DrawNumber(handle, number); //Adding number text to the text handle
}

//Two line display
void TwoLineTextTemplate(struct PiProc* proc) {
    int gwIndex = GetChapterDefinition(gChapterData.chapterIndex)->goalWindowDataType;
    
    Text_Clear(&proc->textA);
    Text_InsertString(&proc->textA, 0x6, TEXT_COLOR_NORMAL, GetStringFromIndex(MGWText[gwIndex].indexA));
    
    Text_Clear(&proc->textB);
    Text_InsertString(&proc->textB, 0x6, TEXT_COLOR_NORMAL, GetStringFromIndex(MGWText[gwIndex].indexB));
    
    proc->bottomPadding = 1;
}

//Display gold only
void GoldTextTemplate(struct PiProc* proc) {
    Text_Clear(&proc->textA); //Clearing text from textA
    InsertNumber(&proc->textA, 0x36, TEXT_COLOR_BLUE, GetPartyGoldAmount());
    
    proc->bottomPadding = 0; //Set to 0 for small window, or 1 for big window. (textB will not be displayed with a small window)
}

//Display gold and current turn
void GoldTurnTextTemplate(struct PiProc* proc) {
    int gwIndex = GetChapterDefinition(gChapterData.chapterIndex)->goalWindowDataType;
    
    Text_Clear(&proc->textA);
    InsertNumber(&proc->textA, 0x36, TEXT_COLOR_BLUE, GetPartyGoldAmount());
    
    Text_Clear(&proc->textB);
    Text_InsertString(&proc->textB, 0xC, TEXT_COLOR_NORMAL, GetStringFromIndex(MGWText[gwIndex].indexB)); //Works the same as InsertNumber, but with regular text
    
    InsertNumber(&proc->textB, 0x30, TEXT_COLOR_BLUE, gChapterData.turnNumber);
    
    proc->bottomPadding = 1;
}
