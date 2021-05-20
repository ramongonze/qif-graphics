#ifndef _guimenu
#define _guimenu

#include <vector>
#include "../../libs/raylib/src/raylib.h"
#include <stdlib.h>
#include <string.h>

using namespace std;

#define REC_BUTTON_OPEN 0
#define REC_BUTTON_SAVE 1
#define REC_BUTTON_EXAMPLES 2
#define REC_BUTTON_HELP 3
#define REC_BUTTON_ABOUT 4
#define OPEN_WINDOW_WIDTH 800
#define OPEN_WINDOW_HEIGHT 400

class GuiMenu{
private:
    /* data */
public:
    GuiMenu();

    // Data
    bool windowErrorActive;

    // Text
    char *buttonOpenText;    // BUTTON: buttonOpen
    char *buttonSaveText;    // BUTTON: buttonSave
    char *buttonExamplesText;    // BUTTON: buttonExamples
    char *buttonHelpText;    // BUTTON: buttonHelp
    char *buttonAboutText;    // BUTTON: buttonAbout
    char *fileNameToOpen;
    char *fileNameToSave;

    // Define controls rectangles
    vector<Rectangle> layoutRecsButtons;
    Rectangle layoutRecsMenu;
};

#endif