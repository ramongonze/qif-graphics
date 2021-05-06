#ifndef _guimenu
#define _guimenu

#include "../../libs/raylib/src/raylib.h"

#define BUTTON_OPEN 0
#define BUTTON_SAVE 1
#define BUTTON_EXAMPLES 2
#define BUTTON_HELP 3
#define BUTTON_ABOUT 4
#define LINE_MENU 5

class GuiMenu{
private:
    /* data */
public:
    // Const text
    const char *buttonOpenText = "Open";    // BUTTON: buttonOpen
    const char *buttonSaveText = "Save";    // BUTTON: buttonSave
    const char *buttonExamplesText = "Examples";    // BUTTON: buttonExamples
    const char *buttonHelpText = "Help";    // BUTTON: buttonHelp
    const char *buttonAboutText = "About";    // BUTTON: buttonAbout

    // Define controls rectangles
    Rectangle layoutRecs[6] = {
        (Rectangle){ 10, 10, 80, 32 },    // Button: buttonOpen
        (Rectangle){ 100, 10, 80, 32 },    // Button: buttonSave
        (Rectangle){ 190, 10, 80, 32 },    // Button: buttonExamples
        (Rectangle){ 280, 10, 80, 32 },    // Button: buttonHelp
        (Rectangle){ 370, 10, 80, 32 },    // Button: buttonAbout
        (Rectangle){ 0, 45, 1045, 10 },    // Line: LineMenu
    };
};

#endif