#include "ModularGoalWindow.h"

//ASMC to set the IconID from event slot 1 to freebyte
void SetMGWIcon() {
    gChapterData._u19 = gEventSlot[1];
}

void DrawGoalWindow(struct GoalWindowProc* proc) {
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
    if (options->iconIndex != 0 && gChapterData._u19 != 0xFF) {
        int iconCoords = TILEMAP_INDEX(options->iconX, options->iconY);
        DrawIcon(TM_FOREGROUND + iconCoords, gChapterData._u19, 0xD000);
        LoadIconPalette(options->iconPal, 0xD);
    }
    
}

//Ease of use function for inserting numbers in textHandles
void InsertNumber(struct TextHandle* handle, int xCursor, int color, int number) {
    Text_SetXCursor(handle, xCursor); //XCursor is how far to the right your text is located
    Text_SetColorId(handle, color);   //Color of the text
    Text_DrawNumber(handle, number);  //Adding number text to the text handle
}

//Two line display
void TwoLineTextTemplate(struct GoalWindowProc* proc) {
    u16 textIndexA = MGWText[gChapterData.chapterIndex].indexA;
    u16 textIndexB = MGWText[gChapterData.chapterIndex].indexB;
    
    Text_Clear(&proc->textA);
    Text_InsertString(&proc->textA, 0x6, TEXT_COLOR_NORMAL, GetStringFromIndex(textIndexA));
    
    Text_Clear(&proc->textB);
    Text_InsertString(&proc->textB, 0x6, TEXT_COLOR_NORMAL, GetStringFromIndex(textIndexB));
    
    proc->bottomPadding = 1;
}

//Display gold only
void GoldTextTemplate(struct GoalWindowProc* proc) {
    Text_Clear(&proc->textA); //Clearing text from textA
    InsertNumber(&proc->textA, 0x36, TEXT_COLOR_BLUE, GetPartyGoldAmount());
    
    //Set to 0 for small window, or 1 for big window. (textB will not be displayed with a small window)
    proc->bottomPadding = 0;
}

//Display gold and current turn
void GoldTurnTextTemplate(struct GoalWindowProc* proc) {
    Text_Clear(&proc->textA);
    InsertNumber(&proc->textA, 0x36, TEXT_COLOR_BLUE, GetPartyGoldAmount());
    
    Text_Clear(&proc->textB);

    //Works the same as InsertNumber, but with regular text
    Text_InsertString(&proc->textB, 0xC, TEXT_COLOR_NORMAL, GetStringFromIndex(0x015D));
    
    InsertNumber(&proc->textB, 0x30, TEXT_COLOR_BLUE, gChapterData.turnNumber);
    
    proc->bottomPadding = 1;
}

void EnemyAndTurnCountTemplate(GoalWindowProc* proc) {
    u16 textIndexA = MGWText[gChapterData.chapterIndex].indexA;
    u16 textIndexB = MGWText[gChapterData.chapterIndex].indexB;

    Text_Clear(&proc->textA);
    Text_Clear(&proc->textB);

    Text_InsertString(&proc->textA, 0x8, TEXT_COLOR_NORMAL, GetStringFromIndex(textIndexA));
    InsertNumber(&proc->textA, 0x34, TEXT_COLOR_BLUE, CountUnitsByFaction(UA_RED));

    if (gChapterData.turnNumber < GetChapterDefinition(gChapterData.chapterIndex)->goalWindowEndTurnNumber) {
        Text_InsertString(&proc->textB, 0x2, TEXT_COLOR_NORMAL, GetStringFromIndex(textIndexB));
        InsertNumber(&proc->textB, 0x20, TEXT_COLOR_BLUE, gChapterData.turnNumber);
        Text_InsertString(&proc->textB, 0x28, TEXT_COLOR_NORMAL, "/");
        InsertNumber(&proc->textB, 0x38, TEXT_COLOR_BLUE, GetChapterDefinition(gChapterData.chapterIndex)->goalWindowEndTurnNumber);
    }
    else {
        Text_InsertString(&proc->textB, 0xC, TEXT_COLOR_GREEN, GetStringFromIndex(0x01C3)); //Last Turn
    }

    proc->bottomPadding = 1;
}

void RealTimeClockDisplayTemplate(GoalWindowProc* proc) {
    Text_Clear(&proc->textA);

    StartClockText(&proc->textA, 0xC, proc);

    proc->bottomPadding = 0;
}
