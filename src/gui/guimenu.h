#ifndef _guimenu
#define _guimenu

#include <vector>
#include "../../libs/raylib/src/raylib.h"
using namespace std;

#define REC_BUTTON_OPEN 0
#define REC_BUTTON_SAVE 1
#define REC_BUTTON_EXAMPLES 2
#define REC_BUTTON_HELP 3
#define REC_BUTTON_ABOUT 4

class GuiMenu{
private:
    /* data */
public:
    GuiMenu();

    // Const text
    const char *buttonOpenText;    // BUTTON: buttonOpen
    const char *buttonSaveText;    // BUTTON: buttonSave
    const char *buttonExamplesText;    // BUTTON: buttonExamples
    const char *buttonHelpText;    // BUTTON: buttonHelp
    const char *buttonAboutText;    // BUTTON: buttonAbout

    // Define controls rectangles
    vector<Rectangle> layoutRecsButtons;
    Rectangle layoutRecsLine;
};

#endif