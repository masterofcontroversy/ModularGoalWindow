#include "ModularGoalWindow.h"

//Adapted from Stan's FE7 version

//TODO: Add a target time argument to count down to
//TODO: Handle negative numbers for the clock text

struct ProcInstruction const ClockTextProcInstruction[] = {
    PROC_LOOP_ROUTINE(UpdateClockText),
    PROC_END,
};

//Helper function to draw a number as two digits
static void DrawNumber2Chars(struct TextHandle* text, u16 number) {
    char digit_a = '0' + __modsi3(number, 10);
    char digit_b = '0' + __divsi3(number, 10);

    Text_DrawChar(text, &digit_a);
    text->xCursor -= 15;
    Text_DrawChar(text, &digit_b);
    text->xCursor -= 15;
}

//Draws formatted time as
void DrawClockText(ClockTextProc* proc) {
    int x = proc->x;
    struct TextHandle* text = proc->text;

    Text_Clear(text);
    
    u16 hours, minutes, seconds;

    //Use chapter start time for start of the timer
    FormatTime(
        (GetGameClock() - gChapterData._u04),
        &hours,
        &minutes,
        &seconds
    );

    Text_SetXCursor(text, x +  0); DrawNumber2Chars(text, hours);
    Text_SetXCursor(text, x +  9); Text_DrawString(text, ":");
    Text_SetXCursor(text, x + 21); DrawNumber2Chars(text, minutes);
    Text_SetXCursor(text, x + 30); Text_DrawString(text, ":");
    Text_SetXCursor(text, x + 42); DrawNumber2Chars(text, seconds);
}

//Refreshes the clock every 60 frames
void UpdateClockText(ClockTextProc* proc) {
    proc->clock += 1;

    if (proc->clock == 60) {
        proc->clock = 0;
        DrawClockText(proc);
    }
}

//Starts and initializes Clock Text Proc
void StartClockText(TextHandle* text, int x, GoalWindowProc* parent) {
    ClockTextProc* proc = ProcStart(ClockTextProcInstruction, parent);
    
    proc->x = x;
    proc->clock = 0;
    proc->text = text;

    DrawClockText(proc);
}
