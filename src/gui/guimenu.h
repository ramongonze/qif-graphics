#ifndef _guimenu
#define _guimenu

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../data.h"
#include "../../libs/raylib/src/raylib.h"

using namespace std;

#define OPEN_WINDOW_WIDTH 800
#define OPEN_WINDOW_HEIGHT 400

#define REC_BUTTON_FILE 0
#define REC_BUTTON_EXAMPLES 1
#define REC_BUTTON_HELP 2

#define BUTTON_FILE_OPTION_FILE 0
#define BUTTON_FILE_OPTION_OPEN 1
#define BUTTON_FILE_OPTION_SAVE 2
#define BUTTON_FILE_OPTION_SAVEAS 3
#define BUTTON_FILE_OPTION_EXIT 4

class GuiMenu{
private:
    /* data */
public:
    GuiMenu();

    // Data
    int dropdownBoxFileActive;
    bool dropdownFileEditMode;

    // Text
    char *buttonFileText;    // BUTTON: buttonOpen
    char *buttonExamplesText;    // BUTTON: buttonExamples
    char *buttonHelpText;    // BUTTON: buttonHelp
    char *fileName;     // Used with file button to open/save files

    // Define controls rectangles
    vector<Rectangle> layoutRecsButtons;
    Rectangle layoutRecsMenu;

    int readQIFFile(vector<char*> &prior, vector<vector<char*>> &channel);        // Parameters are output
    void saveQIFFile(vector<char*> &prior, vector<vector<char*>> &channel, bool createNewFile);
};

#endif