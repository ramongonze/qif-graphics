#ifndef _guimenu
#define _guimenu

#include <vector>
#include "../../libs/raylib/src/raylib.h"
#include <stdlib.h>
#include <string.h>

using namespace std;

#define OPEN_WINDOW_WIDTH 800
#define OPEN_WINDOW_HEIGHT 400

#define REC_BUTTON_OPEN 0
#define REC_BUTTON_EXAMPLES 1
#define REC_BUTTON_HELP 2

class GuiMenu{
private:
    /* data */
public:
    GuiMenu();

    // Data
    bool windowErrorActive;
    int dropdownBoxOpen;
    bool dropDownOpenEditMode;

    // Text
    char *buttonOpenText;    // BUTTON: buttonOpen
    char *buttonExamplesText;    // BUTTON: buttonExamples
    char *buttonHelpText;    // BUTTON: buttonHelp
    char *fileNameToOpen;
    char *fileNameToSave;

    // Define controls rectangles
    vector<Rectangle> layoutRecsButtons;
    Rectangle layoutRecsMenu;
};

#endif