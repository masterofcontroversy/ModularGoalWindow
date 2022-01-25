#include "gbafe.h"

typedef struct ROMChapterData ROMChapterData;
typedef struct goalOptions goalOptions;

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

//Vanilla function for getting ROM chapter data
extern ROMChapterData* GetChapterDefinition(int index);

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

//ASMC to set the IconID from event slot 1 to freebyte
void SetMGWIcon() {
    FreeByte = gEventSlot[1];
}

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
    goalOptions* options = &MGWOptions[gwIndex];

    //If the byte that decides if the template is using a speical character is 0, don't draw any special character
    //Character specs are determined in EA
    if (options->charIndex != 0) {
        int charCoords = TILEMAP_INDEX(options->charX, options->charY);
        DrawSpecialUiChar(
        TM_FOREGROUND + charCoords,
        options->charColor,
        options->charIndex
        );
    }
    //TODO: Fix the palette problem
    //Don't display if template isn't allowed to, or if the icon ID to display is 0xFF
    //Icon location is determined in EA
    if (options->iconIndex != 0 && FreeByte != 0xFF) {
        int iconCoords = TILEMAP_INDEX(options->iconX, options->iconY);
        DrawIcon(TM_FOREGROUND + iconCoords, FreeByte, 0xD000);
        LoadIconPalette(options->iconPal, 0xD);
    }
    
}

//Ease of use function for inserting numbers in textHandles
void InsertNumber(struct TextHandle* handle, int xCursor, int color, int number) {
    Text_SetXCursor(handle, xCursor);   //XCursor is how far to the right your text is located
    Text_SetColorId(handle, color);     //Color of the text
    Text_DrawNumber(handle, number);    //Adding number text to the text handle
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
    
    //Set to 0 for small window, or 1 for big window. (textB will not be displayed with a small window)
    proc->bottomPadding = 0;
}

//Display gold and current turn
void GoldTurnTextTemplate(struct PiProc* proc) {
    int gwIndex = GetChapterDefinition(gChapterData.chapterIndex)->goalWindowDataType;
    
    Text_Clear(&proc->textA);
    InsertNumber(&proc->textA, 0x36, TEXT_COLOR_BLUE, GetPartyGoldAmount());
    
    Text_Clear(&proc->textB);

    //Works the same as InsertNumber, but with regular text
    Text_InsertString(&proc->textB, 0xC, TEXT_COLOR_NORMAL, GetStringFromIndex(MGWText[gwIndex].indexB));
    
    InsertNumber(&proc->textB, 0x30, TEXT_COLOR_BLUE, gChapterData.turnNumber);
    
    proc->bottomPadding = 1;
}
