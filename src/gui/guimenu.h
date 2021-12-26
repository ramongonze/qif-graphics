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
#define BUTTON_GUIDE 3

#define BUTTON_FILE_OPTION_FILE 0
#define BUTTON_FILE_OPTION_OPEN 1
#define BUTTON_FILE_OPTION_SAVE 2
#define BUTTON_FILE_OPTION_SAVEAS 3
#define BUTTON_FILE_OPTION_EXIT 4

#define BUTTON_MODE_OPTION_MODE 0
#define BUTTON_MODE_OPTION_SINGLE 1
#define BUTTON_MODE_OPTION_TWO 2
#define BUTTON_MODE_OPTION_REF 3
#define BUTTON_MODE_OPTION_DP_SINGLE 4
#define BUTTON_MODE_OPTION_DP_TWO 5
#define BUTTON_MODE_OPTION_DP_POST_PROCESS 6
#define NUM_BUTTON_MODE 7

#define BUTTON_EXAMPLES_OPTION_EXAMPLES 0
#define BUTTON_EXAMPLES_OPTION_CH_0 1       // Channel that leaks everything
#define BUTTON_EXAMPLES_OPTION_CH_1 2       // Channel that leaks nothing

#define GS_OPTION_QIF 0
#define GS_OPTION_PRIOR 1
#define GS_OPTION_CHANNELS 2
#define GS_OPTION_HYPER 3
#define GS_OPTION_MODE_SINGLE 4
#define GS_OPTION_MODE_TWO 5
#define GS_OPTION_MODE_REF 6

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
    char buttonModeOptions[NUM_BUTTON_MODE][CHAR_BUFFER_SIZE];
    int ddActive[NUM_BUTTON_MODE];
    bool ddEdit[NUM_BUTTON_MODE];
    bool windowGSActive;

    // Text
    char buttonFileText[CHAR_BUFFER_SIZE];
    char buttonModeText[CHAR_BUFFER_SIZE];
    char buttonExamplesText[CHAR_BUFFER_SIZE];
    char buttonGuideText[CHAR_BUFFER_SIZE];
    char* fileName;     // Used with file button to open/save files

    // Define controls rectangles
    Rectangle recButtons[4];
    Rectangle recMenu;
    Rectangle recGS;
    Rectangle recGSMenu;
    Rectangle recGSPanel;
    
    // Getting started widgets
    Rectangle recScrPan;
    Vector2 scrPanOffset;
    Vector2 scrPanBOffset;
    Vector2 scrPanContent;
    int gsOptionYOffset[7]; // Used to define Scroll Panel bounds
    int gsContentHeight[7]; // Used to set Panel Multibox height

    char gsMenuOptions[CHAR_BUFFER_SIZE];
    int gsMenuScrollIndex;
    int gsMenuActive;

    Texture2D gsImages[7];
    char gsDescriptionTexts[7][CHAR_BUFFER_SIZE*20];
    int imgPadding[7]; // Space between description and image
    char imagesSrc[7][CHAR_BUFFER_SIZE];

    //------------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------------
    
    // Re-define the string of button mode dropdown box
    void updateButtonModeString(int mode);

    // Parameters are output
    int readQIFFile(
        char prior[NUMBER_SECRETS][CHAR_BUFFER_SIZE],
        char channel[NUMBER_CHANNELS][MAX_CHANNEL_OUTPUTS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE],
        int numSecrets[NUMBER_CHANNELS],
        int numOutputs[NUMBER_CHANNELS]
    );

    void saveQIFFile(
        char prior[NUMBER_SECRETS][CHAR_BUFFER_SIZE],
        char channel[NUMBER_CHANNELS][MAX_CHANNEL_OUTPUTS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE],
        int numSecrets[NUMBER_CHANNELS],
        int numOutputs[NUMBER_CHANNELS],
        int mode,
        bool createNewFile);

    void loadGSImages();
};

#endif