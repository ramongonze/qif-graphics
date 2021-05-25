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

    //------------------------------------------------------------------------------------
    // Attributes
    //------------------------------------------------------------------------------------

    // Data
    int dropdownBoxFileActive;
    bool dropdownFileEditMode;

    // Text
    char buttonFileText[CHAR_BUFFER_SIZE];
    char buttonExamplesText[CHAR_BUFFER_SIZE];
    char buttonHelpText[CHAR_BUFFER_SIZE];
    char fileName[10*CHAR_BUFFER_SIZE];     // Used with file button to open/save files

    // Define controls rectangles
    Rectangle layoutRecsButtons[3];
    Rectangle layoutRecsMenu;

    //------------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------------

    int readQIFFile(char prior[NUMBER_SECRETS][CHAR_BUFFER_SIZE], char channel[NUMBER_SECRETS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE], int* newNumOutputs);        // Parameters are output
    void saveQIFFile(char prior[NUMBER_SECRETS][CHAR_BUFFER_SIZE], char channel[NUMBER_SECRETS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE], bool createNewFile);
};

#endif