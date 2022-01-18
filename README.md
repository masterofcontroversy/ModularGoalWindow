# Modular Goal Window

## Credits:
Many thanks to Stan for making the base that I built off of. Without his work, this hack wouldn't exist.

## Instructions (I haven't made this Windows friendly yet. I'll do that soon):
Place a clean ROM of Fire Emblem: The Sacred Stones (U) in the root directory and name it "FE8_clean.gba"
Run MakeHack.sh

In ROMBuildfile.event, there's a segment that orgs to the prologue's goal window selection byte. Change that to the index that matches the template in C_Wizardry/ModularGoalWindow/Installer.event that you want.

In the installer, there are also options to decide what text IDs you want for each custom template, and what where you want to display one special character, and one icon (See the macros for specifics).

You can edit or add templates in C_Wizardry/ModularGoalWindow/ModularGoalWindow.c (Some C hacking knowledge is recommended)

### Current Custom Templates:
-Simple two line display.  
-Gold display.  
-Gold and turn count display.  

## TODO:
-Make a table to change displayed text by chapter.  
-Add a timer template (Along with a time based event trigger to match).  
-Figure out a way to dynamically change the icon palette.  
-Optimize and improve the implementation.