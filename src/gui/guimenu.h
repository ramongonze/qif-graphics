#ifndef _guimenu
#define _guimenu

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../../libs/raylib/src/raylib.h"
#include "../data.h"
#include "guiprior.h"
#include "guichannel.h"

using namespace std;

#define OPEN_WINDOW_WIDTH 800
#define OPEN_WINDOW_HEIGHT 400

#define BUTTON_FILE 0
#define BUTTON_MODE 1
#define BUTTON_EXAMPLES 2
#define BUTTON_HELP 3

#define BUTTON_FILE_OPTION_FILE 0
#define BUTTON_FILE_OPTION_OPEN 1
#define BUTTON_FILE_OPTION_SAVE 2
#define BUTTON_FILE_OPTION_SAVEAS 3
#define BUTTON_FILE_OPTION_EXIT 4

#define BUTTON_MODE_OPTION_MODE 0
#define BUTTON_MODE_OPTION_SINGLE 1
#define BUTTON_MODE_OPTION_TWO 2
#define BUTTON_MODE_OPTION_REF 3

#define BUTTON_EXAMPLES_OPTION_EXAMPLES 0
#define BUTTON_EXAMPLES_OPTION_CH_0 1       // Channel that leaks everything
#define BUTTON_EXAMPLES_OPTION_CH_1 2       // Channel that leaks nothing

#define BUTTON_HELP_OPTION_HELP 0
#define BUTTON_HELP_OPTION_GETTING_STARTED 1
#define BUTTON_HELP_OPTION_ABOUT 2

#define GS_PRIOR 0
#define GS_CHANNEL 1
#define GS_HYPER 2
#define GS_VISUALIZATION 3
#define GS_REFINEMENT 4

class GuiMenu{
private:
    /* data */
public:
    GuiMenu();
    GuiMenu(int windowWidth, int windowHeight);

    //------------------------------------------------------------------------------------
    // Attributes
    //------------------------------------------------------------------------------------

    // Data
    int windowWidth;
    int windowHeight;
    int dropdownBoxActive[4];
    bool dropdownEditMode[4];
    bool windowGettingStartedActive;

    // Text
    char buttonFileText[CHAR_BUFFER_SIZE];
    char buttonModeText[CHAR_BUFFER_SIZE];
    char buttonExamplesText[CHAR_BUFFER_SIZE];
    char buttonHelpText[CHAR_BUFFER_SIZE];
    char* fileName;     // Used with file button to open/save files

    // Define controls rectangles
    Rectangle layoutRecsButtons[4];
    Rectangle layoutRecsMenu;
    Rectangle layoutRecsGettingStarted;
    Rectangle layoutRecsGettingStartedMenu;
    Rectangle layoutRecsGettingStartedPanel;

    // Getting started widgets
    char gettingStartedMenuOptions[CHAR_BUFFER_SIZE];
    int gettingStartedMenuScrollIndex;
    int gettingStartedMenuActive;

    Texture2D gsImages[5];
    char gsDescriptionTexts[5][CHAR_BUFFER_SIZE*5];
    int imgPadding[5]; // Space between description and image

    //------------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------------
    int readQIFFile(char prior[NUMBER_SECRETS][CHAR_BUFFER_SIZE], char channel[NUMBER_SECRETS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE], int* newNumOutputs);        // Parameters are output
    void saveQIFFile(char prior[NUMBER_SECRETS][CHAR_BUFFER_SIZE], char channel[NUMBER_SECRETS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE], int numOutputs, bool createNewFile);
    void loadGSImages();
};

#endif